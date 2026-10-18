#include <gtest/gtest.h>

#include <Window/Window.h>

#include <wrl.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <d3d12sdklayers.h>
#include <dxgidebug.h>
#include <d3dcompiler.h>

#include <vector>
#include <array>
#include <chrono>
#include <fstream>

#include <tiny_obj_loader.h>
#include <stb_image.h>
#include <Helpers/Vertex.h>
#include <Matrix/Matrix4.h>

class DX12Test : public testing::Test {};

template <typename T>
using MPtr = Microsoft::WRL::ComPtr<T>;

static void DebugCallback(D3D12_MESSAGE_CATEGORY _category, D3D12_MESSAGE_SEVERITY _severity, D3D12_MESSAGE_ID _id, LPCSTR _description, void* _context)
{
	(void*)_category, _id, _context;

	switch (_severity)
	{
	case D3D12_MESSAGE_SEVERITY_CORRUPTION:
	case D3D12_MESSAGE_SEVERITY_ERROR:
	case D3D12_MESSAGE_SEVERITY_WARNING:
	case D3D12_MESSAGE_SEVERITY_MESSAGE:
		std::cout << _severity << " " << _description << "\n\n";
		break;
	}
}

static std::vector<char> GetShaderData(const std::string& _path)
{
	std::ifstream file(_path, std::ios::ate | std::ios::binary);

	if (!file.is_open())
		return std::vector<char>();

	size_t fileSize = (size_t)file.tellg();
	std::vector<char> buffer(fileSize);

	file.seekg(0);
	file.read(buffer.data(), fileSize);
	file.close();

	return buffer;
}

void DeviceWaitIdle(ID3D12CommandQueue* _queue, ID3D12Fence* _fence, uint32_t& _value, HANDLE _event)
{
	_queue->Signal(_fence, _value);

	_fence->SetEventOnCompletion(_value, _event);
	WaitForSingleObjectEx(_event, INFINITE, false);

	++_value;
};

TEST(DX12Test, LoopTest)
{
	Window window(1920, 1080, "Raw DX12 Window");

	const uint32_t MAX_FRAMES_IN_FLIGHT = 3;
	const DXGI_FORMAT scFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
	const DXGI_FORMAT depthFormat = DXGI_FORMAT_D32_FLOAT;

	// Instance
	MPtr<IDXGIFactory6> factory{};
	{
		UINT factoryFlags = 0;
		MPtr<ID3D12Debug1> debugController{};
		HRESULT result = D3D12GetDebugInterface(IID_PPV_ARGS(&debugController));

		if (SUCCEEDED(result))
		{
			debugController->EnableDebugLayer();
			debugController->SetEnableGPUBasedValidation(true);
		}

		MPtr<IDXGIInfoQueue> dxgiInfoQueue = nullptr;
		const HRESULT debugInterfaceResult = DXGIGetDebugInterface1(0, IID_PPV_ARGS(&dxgiInfoQueue));

		if (SUCCEEDED(debugInterfaceResult))
		{
			dxgiInfoQueue->SetBreakOnSeverity(DXGI_DEBUG_ALL, DXGI_INFO_QUEUE_MESSAGE_SEVERITY_CORRUPTION, true);
			dxgiInfoQueue->SetBreakOnSeverity(DXGI_DEBUG_ALL, DXGI_INFO_QUEUE_MESSAGE_SEVERITY_ERROR, true);
			dxgiInfoQueue->SetBreakOnSeverity(DXGI_DEBUG_ALL, DXGI_INFO_QUEUE_MESSAGE_SEVERITY_WARNING, true);
		}

		factoryFlags |= DXGI_CREATE_FACTORY_DEBUG;

		result = CreateDXGIFactory2(factoryFlags, IID_PPV_ARGS(&factory));
	}

	// Device
	MPtr<ID3D12Device> device{};
	DWORD validationLayerCookie = 0;
	{
		MPtr<IDXGIAdapter3> adapter{};

		HRESULT result = factory->EnumAdapterByGpuPreference(0, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&adapter));
		result = D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&device));

		MPtr<ID3D12InfoQueue1> infoQueue = nullptr;
		result = device->QueryInterface(IID_PPV_ARGS(&infoQueue));

		if (SUCCEEDED(result))
		{
			infoQueue->RegisterMessageCallback(DebugCallback, D3D12_MESSAGE_CALLBACK_IGNORE_FILTERS, nullptr, &validationLayerCookie);
			infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);
			infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);
			infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, true);
		}
	}

	// Queue
	MPtr<ID3D12CommandQueue> queue{};
	{
		const D3D12_COMMAND_QUEUE_DESC queueDesc{
			.Type = D3D12_COMMAND_LIST_TYPE_DIRECT,
			.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE,
		};

		device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&queue));
	}

	// Device Sync
	HANDLE deviceEvent;
	MPtr<ID3D12Fence> deviceFence;
	uint32_t deviceValue = 1u;
	{
		deviceEvent = CreateEvent(nullptr, false, false, nullptr);
		device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&deviceFence));
	}

	// SwapChain
	MPtr<IDXGISwapChain3> swapChain{};
	uint32_t swapchainFrame = 0u;
	{
		const DXGI_SWAP_CHAIN_DESC1 scDesc{
			.Width = 1920,
			.Height = 1080,
			.Format = scFormat,
			.Stereo = false,
			.SampleDesc = {
				.Count = 1,
				.Quality = 0
			},
			.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT,
			.BufferCount = MAX_FRAMES_IN_FLIGHT,
			.Scaling = DXGI_SCALING_STRETCH,
			.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD,
			.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED,
			.Flags = 0
		};

		MPtr<IDXGISwapChain1> sc1{};
		factory->CreateSwapChainForHwnd(queue.Get(), window.GetHandle(), &scDesc, nullptr, nullptr, &sc1);
		sc1.As(&swapChain);
	}

	// SwapChain images
	std::array<MPtr<ID3D12Resource>, MAX_FRAMES_IN_FLIGHT> scImages{};
	{
		for (uint32_t i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i)
		{
			swapChain->GetBuffer(i, IID_PPV_ARGS(&scImages[i]));
		}
	}

	// SwapChain synchro
	HANDLE scEvent{};
	MPtr<ID3D12Fence> scFence{};
	std::array<uint32_t, MAX_FRAMES_IN_FLIGHT> scValues{};
	{
		scEvent = CreateEvent(nullptr, false, false, nullptr);

		device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&scFence));
	}

	// RootSignature
	MPtr< ID3D12RootSignature> rootSignature{};
	{
		const D3D12_DESCRIPTOR_RANGE1 textureRange{
			.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV,
			.NumDescriptors = 1,
			.BaseShaderRegister = 0,
			.RegisterSpace = 0,
			.Flags = D3D12_DESCRIPTOR_RANGE_FLAG_NONE,
			.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND
		};

		const D3D12_DESCRIPTOR_RANGE1 samplerRange{
			.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER,
			.NumDescriptors = 1,
			.BaseShaderRegister = 0,
			.RegisterSpace = 0,
			.Flags = D3D12_DESCRIPTOR_RANGE_FLAG_NONE,
			.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND
		};

		const D3D12_ROOT_PARAMETER1 rootParams[]{
		{
			.ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS,
			.Constants{
				.ShaderRegister = 0,
				.RegisterSpace = 0,
				.Num32BitValues = 32
			},
			.ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX
		},
		{
			.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE,
			.DescriptorTable{
				.NumDescriptorRanges = 1,
				.pDescriptorRanges = &textureRange
			},
			.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL
		},
		{
			.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE,
			.DescriptorTable{
				.NumDescriptorRanges = 1,
				.pDescriptorRanges = &samplerRange
			},
			.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL
		}
		};

		const D3D12_VERSIONED_ROOT_SIGNATURE_DESC rootDesc{
			.Version = D3D_ROOT_SIGNATURE_VERSION_1_1,
			.Desc_1_1{
				.NumParameters = _countof(rootParams),
				.pParameters = rootParams,
				.NumStaticSamplers = 0,
				.pStaticSamplers = nullptr,
				.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT
			}
		};

		MPtr<ID3D10Blob> signature{};
		MPtr<ID3D10Blob> error{};

		HRESULT result = D3D12SerializeVersionedRootSignature(&rootDesc, &signature, &error);
		result = device->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&rootSignature));
	}

	// Shaders
	MPtr<ID3DBlob> vertShaderBlob{};
	MPtr<ID3DBlob> pixShaderBlob{};
	{
		std::vector<char> vertShaderData = GetShaderData("Assets/Shaders/Unlit/DX12/Unlit_VERT.cso");
		HRESULT result = D3DCreateBlob(vertShaderData.size(), &vertShaderBlob);
		memcpy(vertShaderBlob->GetBufferPointer(), vertShaderData.data(), vertShaderData.size());

		std::vector<char> pixShaderData = GetShaderData("Assets/Shaders/Unlit/DX12/Unlit_PIX.cso");
		result = D3DCreateBlob(pixShaderData.size(), &pixShaderBlob);
		memcpy(pixShaderBlob->GetBufferPointer(), pixShaderData.data(), pixShaderData.size());
	}

	// Pipeline
	MPtr<ID3D12PipelineState> pipelineState{};
	{
		const D3D12_VIEWPORT viewport{
			.TopLeftX = 0.f,
			.TopLeftY = 0.f,
			.Width = 1920.f,
			.Height = 1080.f,
			.MinDepth = 0.f,
			.MaxDepth = 1.f
		};

		const D3D12_RECT scissor{
			.left = 0,
			.top = 0,
			.right = 1920,
			.bottom = 1080
		};

		// HARDCODED
		const D3D12_DESCRIPTOR_RANGE1 textureRange{
			.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV,
			.NumDescriptors = 1,
			.BaseShaderRegister = 0,
			.RegisterSpace = 0,
			.Flags = D3D12_DESCRIPTOR_RANGE_FLAG_NONE,
			.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND
		};

		const D3D12_DESCRIPTOR_RANGE1 samplerRange{
			.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER,
			.NumDescriptors = 1,
			.BaseShaderRegister = 0,
			.RegisterSpace = 0,
			.Flags = D3D12_DESCRIPTOR_RANGE_FLAG_NONE,
			.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND
		};

		const D3D12_RENDER_TARGET_BLEND_DESC rtBlendDesc{
			.BlendEnable = false,
			.LogicOpEnable = false,

			.SrcBlend = D3D12_BLEND_ONE,
			.DestBlend = D3D12_BLEND_ZERO,
			.BlendOp = D3D12_BLEND_OP_ADD,

			.SrcBlendAlpha = D3D12_BLEND_ONE,
			.DestBlendAlpha = D3D12_BLEND_ZERO,
			.BlendOpAlpha = D3D12_BLEND_OP_ADD,

			.LogicOp = D3D12_LOGIC_OP_NOOP,

			.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL
		};

		const D3D12_BLEND_DESC blendDesc{
			.AlphaToCoverageEnable = false,
			.IndependentBlendEnable = false,

			.RenderTarget{
				rtBlendDesc
			}
		};

		const D3D12_RASTERIZER_DESC rasterizerDesc{
			.FillMode = D3D12_FILL_MODE_SOLID,
			.CullMode = D3D12_CULL_MODE_BACK,
			.FrontCounterClockwise = FALSE,
			.DepthBias = D3D12_DEFAULT_DEPTH_BIAS,
			.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP,
			.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS,
			.DepthClipEnable = TRUE,
			.MultisampleEnable = FALSE,
			.AntialiasedLineEnable = FALSE,
			.ForcedSampleCount = 0,
			.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF
		};

		const D3D12_DEPTH_STENCIL_DESC depthStencDesc{
			.DepthEnable = TRUE,
				.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL,
				.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL,
				.StencilEnable = FALSE,
				.StencilReadMask = D3D12_DEFAULT_STENCIL_READ_MASK,
				.StencilWriteMask = D3D12_DEFAULT_STENCIL_WRITE_MASK,
				.FrontFace{
					.StencilFailOp = D3D12_STENCIL_OP_KEEP,
					.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP,
					.StencilPassOp = D3D12_STENCIL_OP_KEEP,
					.StencilFunc = D3D12_COMPARISON_FUNC_ALWAYS,
			},
			.BackFace{
				.StencilFailOp = D3D12_STENCIL_OP_KEEP,
				.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP,
				.StencilPassOp = D3D12_STENCIL_OP_KEEP,
				.StencilFunc = D3D12_COMPARISON_FUNC_ALWAYS,
			}
		};

		const D3D12_INPUT_ELEMENT_DESC inputDescs[]{
			{
				.SemanticName = "POSITION",
				.SemanticIndex = 0,
				.Format = DXGI_FORMAT_R32G32B32_FLOAT,
				.InputSlot = 0,
				.AlignedByteOffset = offsetof(Vertex, position),
				.InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
				.InstanceDataStepRate = 0
			},
			{
				.SemanticName = "NORMAL",
				.SemanticIndex = 0,
				.Format = DXGI_FORMAT_R32G32B32_FLOAT,
				.InputSlot = 0,
				.AlignedByteOffset = offsetof(Vertex, normal),
				.InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
				.InstanceDataStepRate = 0
			},
			{
				.SemanticName = "TEXCOORD",
				.SemanticIndex = 0,
				.Format = DXGI_FORMAT_R32G32_FLOAT,
				.InputSlot = 0,
				.AlignedByteOffset = offsetof(Vertex, textCoords),
				.InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
				.InstanceDataStepRate = 0
			}
		};

		const D3D12_INPUT_LAYOUT_DESC inputDesc{
			.pInputElementDescs = inputDescs,
			.NumElements = _countof(inputDescs)
		};

		const D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineDesc{
			.pRootSignature = rootSignature.Get(),

			.VS{
				.pShaderBytecode = vertShaderBlob.Get()->GetBufferPointer(),
				.BytecodeLength = vertShaderBlob.Get()->GetBufferSize()
			},
			.PS{
				.pShaderBytecode = pixShaderBlob.Get()->GetBufferPointer(),
				.BytecodeLength = pixShaderBlob.Get()->GetBufferSize()
			},

			.StreamOutput{},
			.BlendState = blendDesc,
			.SampleMask = UINT_MAX,

			.RasterizerState = rasterizerDesc,
			.DepthStencilState = depthStencDesc,
			.InputLayout = inputDesc,

			.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE,

			.NumRenderTargets = 1,
			.RTVFormats{
				DXGI_FORMAT_R8G8B8A8_UNORM
			},
			.DSVFormat = depthFormat,

			.SampleDesc{
				.Count = 1,
				.Quality = 0
			},

			.NodeMask = 0,

			.CachedPSO{
				.pCachedBlob = nullptr,
				.CachedBlobSizeInBytes = 0
			},

			.Flags = D3D12_PIPELINE_STATE_FLAG_NONE
		};

		device->CreateGraphicsPipelineState(&graphicsPipelineDesc, IID_PPV_ARGS(&pipelineState));
	}

	// Render targets
	MPtr<ID3D12DescriptorHeap> scRTVHeap{};
	MPtr<ID3D12Resource> scDepth{};
	MPtr<ID3D12DescriptorHeap> sdepthRTVHeap{};
	{
		// SwapChain images
		{
			const D3D12_DESCRIPTOR_HEAP_DESC descriptorDesc{
				.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV,
				.NumDescriptors = MAX_FRAMES_IN_FLIGHT,
				.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE,
				.NodeMask = 0,
			};

			device->CreateDescriptorHeap(&descriptorDesc, IID_PPV_ARGS(&scRTVHeap));

			D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = scRTVHeap->GetCPUDescriptorHandleForHeapStart();
			const UINT rtvOffset = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

			for (uint32_t i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i)
			{
				device->CreateRenderTargetView(scImages[i].Get(), nullptr, rtvHandle);
				rtvHandle.ptr += rtvOffset;
			}
		}

		// Depth
		{
			const D3D12_RESOURCE_DESC depthDesc{
				.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D,
				.Alignment = 0,
				.Width = 1920,
				.Height = 1080,
				.DepthOrArraySize = 1,
				.MipLevels = 1,
				.Format = depthFormat,
				.SampleDesc{ 1, 0 },
				.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN,
				.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL,
			};

			const D3D12_HEAP_PROPERTIES depthHeap{
				.Type = D3D12_HEAP_TYPE_DEFAULT,
				.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN,
				.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN,
				.CreationNodeMask = 1,
				.VisibleNodeMask = 1,
			};

			const D3D12_CLEAR_VALUE depthClear{ .Format = depthFormat, .DepthStencil = { 1.0f, 0 } };
			device->CreateCommittedResource(&depthHeap, D3D12_HEAP_FLAG_NONE, &depthDesc, D3D12_RESOURCE_STATE_DEPTH_WRITE, &depthClear, IID_PPV_ARGS(&scDepth));

			const D3D12_DESCRIPTOR_HEAP_DESC descriptorDesc{
				.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV,
				.NumDescriptors = 1,
				.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE,
				.NodeMask = 0,
			};

			device->CreateDescriptorHeap(&descriptorDesc, IID_PPV_ARGS(&sdepthRTVHeap));
			device->CreateDepthStencilView(scDepth.Get(), nullptr, sdepthRTVHeap->GetCPUDescriptorHandleForHeapStart());
		}
	}

	// Commands
	std::array<MPtr<ID3D12CommandAllocator>, MAX_FRAMES_IN_FLIGHT> cmdAllocs{};
	MPtr<ID3D12GraphicsCommandList1> cmdList{};
	{
		for (uint32_t i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i)
		{
			device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&cmdAllocs[i]));
		}

		device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, cmdAllocs[0].Get(), nullptr, IID_PPV_ARGS(&cmdList));
		cmdList->Close();
	}

	// Buffers (Memory shared, require more work to be in only GPU)
	MPtr<ID3D12Resource> vertexBuffer{};
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};

	MPtr<ID3D12Resource> indexBuffer{};
	D3D12_INDEX_BUFFER_VIEW indexBufferView{};

	size_t indexCount = 0;
	{
		// Loading
		tinyobj::attrib_t attrib{};
		std::vector<tinyobj::shape_t> shapes{};
		std::vector<tinyobj::material_t> materials{};
		tinyobj::LoadObj(&attrib, &shapes, &materials, nullptr, nullptr, "Assets/Meshes/Knight.obj");

		std::vector<Vertex> vertices{};
		std::vector<uint16_t> indices{};

		for (const tinyobj::index_t& index : shapes[0].mesh.indices)
		{
			const Vertex v{
				.position = { attrib.vertices[index.vertex_index * 3], attrib.vertices[index.vertex_index * 3 + 1], attrib.vertices[index.vertex_index * 3 + 2] },
				.normal = { attrib.normals[index.normal_index * 3], attrib.normals[index.normal_index * 3 + 1], attrib.normals[index.normal_index * 3 + 2] },
				.textCoords = { attrib.texcoords[index.texcoord_index * 2], attrib.texcoords[index.texcoord_index * 2 + 1] }
			};

			vertices.push_back(v);
			indices.push_back(static_cast<uint16_t>(indices.size()));
		}

		indexCount = indices.size();

		// Heap can be UPLOAD (CPU -> GPU) | DEFAULT (GPU) | READBACK (GPU -> CPU)
		const D3D12_HEAP_PROPERTIES heap{
			.Type = D3D12_HEAP_TYPE_UPLOAD,
		};

		const D3D12_RESOURCE_DESC vertexDesc{
			.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER,
			.Alignment = 0,
			.Width = vertices.size() * sizeof(Vertex),
			.Height = 1,
			.DepthOrArraySize = 1,
			.MipLevels = 1,
			.Format = DXGI_FORMAT_UNKNOWN,
			.SampleDesc = { 1, 0 },
			.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR,
			.Flags = D3D12_RESOURCE_FLAG_NONE,
		};

		device->CreateCommittedResource(&heap, D3D12_HEAP_FLAG_NONE, &vertexDesc, D3D12_RESOURCE_STATE_COMMON, nullptr, IID_PPV_ARGS(&vertexBuffer));

		// Upload
		{
			D3D12_RANGE mapRange{ 0, 0 };
			void* data{};
			vertexBuffer->Map(0, &mapRange, &data);
			std::memcpy(data, vertices.data(), vertices.size() * sizeof(Vertex));
			vertexBuffer->Unmap(0, nullptr);
		}

		vertexBufferView = D3D12_VERTEX_BUFFER_VIEW{
			.BufferLocation = vertexBuffer->GetGPUVirtualAddress(),
			.SizeInBytes = static_cast<UINT>(vertexDesc.Width),
			.StrideInBytes = sizeof(Vertex)
		};

		const D3D12_RESOURCE_DESC indexDesc{
			.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER,
			.Alignment = 0,
			.Width = sizeof(uint16_t) * indices.size(),
			.Height = 1,
			.DepthOrArraySize = 1,
			.MipLevels = 1,
			.Format = DXGI_FORMAT_UNKNOWN,
			.SampleDesc = { 1, 0 },
			.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR,
			.Flags = D3D12_RESOURCE_FLAG_NONE,
		};

		device->CreateCommittedResource(&heap, D3D12_HEAP_FLAG_NONE, &indexDesc, D3D12_RESOURCE_STATE_COMMON, nullptr, IID_PPV_ARGS(&indexBuffer));

		// Upload
		{
			D3D12_RANGE mapRange{ 0, 0 };
			void* data{};
			indexBuffer->Map(0, &mapRange, &data);
			std::memcpy(data, indices.data(), indices.size() * sizeof(uint16_t));
			indexBuffer->Unmap(0, nullptr);
		}

		indexBufferView = D3D12_INDEX_BUFFER_VIEW{
			.BufferLocation = indexBuffer->GetGPUVirtualAddress(),
			.SizeInBytes = static_cast<UINT>(indexDesc.Width),
			.Format = DXGI_FORMAT_R16_UINT
		};
	}
	
	cmdList->Reset(cmdAllocs[0].Get(), nullptr);

	// Texture
	MPtr<ID3D12Resource> texture{};
	{
		stbi_set_flip_vertically_on_load(true);

		int width, height, channels;
		unsigned char* textData = stbi_load("Assets/Textures/Knight.png", &width, &height, &channels, STBI_rgb_alpha);

		// Heap can be UPLOAD (CPU -> GPU) | DEFAULT (GPU) | READBACK (GPU -> CPU)
		const D3D12_HEAP_PROPERTIES heap{
			.Type = D3D12_HEAP_TYPE_DEFAULT,
		};

		const D3D12_RESOURCE_DESC textureDesc{
			.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D,
			.Alignment = 0,
			.Width = static_cast<UINT64>(width),
			.Height = static_cast<UINT64>(height),
			.DepthOrArraySize = 1,
			.MipLevels = 1,
			.Format = DXGI_FORMAT_R8G8B8A8_UNORM,
			.SampleDesc = { 1, 0 },
			.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN,
			.Flags = D3D12_RESOURCE_FLAG_NONE,
		};

		device->CreateCommittedResource(&heap, D3D12_HEAP_FLAG_NONE, &textureDesc, D3D12_RESOURCE_STATE_COPY_DEST, nullptr, IID_PPV_ARGS(&texture));

		// Upload
		{
			MPtr<ID3D12Resource> stagingBuffer{};

			const D3D12_HEAP_PROPERTIES stagingHeap{
				.Type = D3D12_HEAP_TYPE_UPLOAD,
			};

			const D3D12_RESOURCE_DESC stagingDesc{
				.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER,
				.Alignment = 0,
				.Width = static_cast<UINT64>(width * height * channels),
				.Height = 1,
				.DepthOrArraySize = 1,
				.MipLevels = 1,
				.Format = DXGI_FORMAT_UNKNOWN,
				.SampleDesc = { .Count = 1, .Quality = 0 },
				.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR,
				.Flags = D3D12_RESOURCE_FLAG_NONE,
			};

			device->CreateCommittedResource(&stagingHeap, D3D12_HEAP_FLAG_NONE, &stagingDesc, D3D12_RESOURCE_STATE_COPY_SOURCE, nullptr, IID_PPV_ARGS(&stagingBuffer));

			D3D12_RANGE mapRange{ 0, 0 };
			void* data{};
			stagingBuffer->Map(0, &mapRange, &data);
			std::memcpy(data, textData, width * height * channels);
			stagingBuffer->Unmap(0, nullptr);

			const D3D12_RESOURCE_DESC textDesc = texture->GetDesc();

			const D3D12_TEXTURE_COPY_LOCATION src{
				.pResource = stagingBuffer.Get(),
				.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT,
				.PlacedFootprint{
					.Offset = 0u,
					.Footprint{
						.Format = textDesc.Format,
						.Width = static_cast<UINT>(width),
						.Height = static_cast<UINT>(height),
						.Depth = 1,
						.RowPitch = static_cast<UINT>(width * channels),
					}
				}
			};

			const D3D12_TEXTURE_COPY_LOCATION dst{
				.pResource = texture.Get(),
				.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX,
				.SubresourceIndex = 0
			};

			cmdList->CopyTextureRegion(&dst, 0, 0, 0, &src, nullptr);

			const D3D12_RESOURCE_BARRIER barrier{
				.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION,
				.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE,
				.Transition = {
					.pResource = texture.Get(),
					.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES,
					.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST,
					.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
				},
			};

			cmdList->ResourceBarrier(1, &barrier);

			cmdList->Close();

			ID3D12CommandList* cmdLists[] = { cmdList.Get() };
			queue->ExecuteCommandLists(1, cmdLists);

			DeviceWaitIdle(queue.Get(), deviceFence.Get(), deviceValue, deviceEvent);
		}
	}

	// Descriptor heaps
	MPtr<ID3D12DescriptorHeap> textureDescriptor{};
	MPtr<ID3D12DescriptorHeap> samplerDescriptor{};
	{
		const D3D12_DESCRIPTOR_HEAP_DESC texDesc{
			.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV,
			.NumDescriptors = 1,
			.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE
		};

		device->CreateDescriptorHeap(&texDesc, IID_PPV_ARGS(&textureDescriptor));

		const D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{
			.Format = DXGI_FORMAT_R8G8B8A8_UNORM,
			.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D,
			.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING,
			.Texture2D{
				.MipLevels = 1
			}
		};

		device->CreateShaderResourceView(texture.Get(), &srvDesc, textureDescriptor->GetCPUDescriptorHandleForHeapStart());

		const D3D12_DESCRIPTOR_HEAP_DESC samplerDescriptorDesc{
			.Type = D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER,
			.NumDescriptors = 1,
			.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE
		};

		device->CreateDescriptorHeap(&samplerDescriptorDesc, IID_PPV_ARGS(&samplerDescriptor));

		const D3D12_SAMPLER_DESC samplerDesc{
			.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR,
			.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP,
			.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP,
			.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP,
			.MipLODBias = 0.0f,
			.MaxAnisotropy = 1,
			.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER,
			.BorderColor = { 0.f, 0.f, 0.f, 0.f },
			.MinLOD = 0.0f,
			.MaxLOD = D3D12_FLOAT32_MAX
		};

		device->CreateSampler(&samplerDesc, samplerDescriptor->GetCPUDescriptorHandleForHeapStart());
	}

	// Loop
	{
		float rotation = 0.f;
		auto lastTime = std::chrono::high_resolution_clock::now();

		while (!window.WindowShouldClose())
		{
			auto currentTime = std::chrono::high_resolution_clock::now();
			float dt = std::chrono::duration<float>(currentTime - lastTime).count();
			lastTime = currentTime;

			// Update
			{
				window.UpdateWindow();
			}

			// Render
			{
				// SwapChain Synchro
				{
					const UINT pFence = scValues[swapchainFrame];
					swapchainFrame = swapChain->GetCurrentBackBufferIndex();
					const UINT cFence = scValues[swapchainFrame];

					if (scFence->GetCompletedValue() < cFence)
					{
						scFence->SetEventOnCompletion(cFence, scEvent);
						WaitForSingleObjectEx(scEvent, INFINITE, FALSE);
					}

					scValues[swapchainFrame] = pFence + 1;
				}

				// Draw
				{
					// Command Setup
					{
						MPtr<ID3D12CommandAllocator> cmdAlloc = cmdAllocs[swapchainFrame];
						cmdAlloc->Reset();
						cmdList->Reset(cmdAlloc.Get(), nullptr);
					}

					MPtr<ID3D12Resource> currentImage = scImages[swapchainFrame];

					// First barrier
					{
						const D3D12_RESOURCE_BARRIER barrier{
							.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION,
							.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE,
							.Transition = {
								.pResource = currentImage.Get(),
								.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES,
								.StateBefore = D3D12_RESOURCE_STATE_COMMON,
								.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET,
							},
						};

						cmdList->ResourceBarrier(1, &barrier);
					}


					D3D12_CPU_DESCRIPTOR_HANDLE scImageHandle = scRTVHeap->GetCPUDescriptorHandleForHeapStart();
					const UINT rtvOffset = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
					scImageHandle.ptr += rtvOffset * swapchainFrame;

					const D3D12_CPU_DESCRIPTOR_HANDLE depthHandle = sdepthRTVHeap->GetCPUDescriptorHandleForHeapStart();

					float clearColor[] = { 0.0f, 0.0f, 0.2f, 1.0f };

					cmdList->OMSetRenderTargets(1, &scImageHandle, FALSE, &depthHandle);
					cmdList->ClearRenderTargetView(scImageHandle, clearColor, 0, nullptr);
					cmdList->ClearDepthStencilView(depthHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

					const D3D12_VIEWPORT viewport{
						.TopLeftX = 0,
						.TopLeftY = 0,
						.Width = float(window.GetWidth()),
						.Height = float(window.GetHeight()),
						.MinDepth = 0.0f,
						.MaxDepth = 1.0f,
					};

					const D3D12_RECT scissor{
						.left = 0,
						.top = 0,
						.right = static_cast<LONG>(window.GetWidth()),
						.bottom = static_cast<LONG>(window.GetHeight()),
					};

					cmdList->RSSetViewports(1, &viewport);
					cmdList->RSSetScissorRects(1, &scissor);

					cmdList->SetGraphicsRootSignature(rootSignature.Get());

					cmdList->SetPipelineState(pipelineState.Get());

					Matrix4f M;
					Matrix4f VP;

					struct
					{
						Matrix4f Model;
						Matrix4f ProjectionView;
					} constants;

					const Matrix4f V = VP.ViewMatrix(
						Vector3f(0.f, 0.f, 0.f),
						Vector3f(0.f, 0.f, 1.f),
						Vector3f(0.f, 1.f, 0.f)
					);

					const Matrix4f P = VP.PerspectiveProjection(
						0.1f,
						100.f,
						float(window.GetWidth()) / float(window.GetHeight()),
						90.f * KM_DEG_2_RAD(float)
					);

					constants.ProjectionView = (P * V);
					constants.Model = M.TRS(M.Translation(0.f, -2.5f, -5.f), M.YRotation(rotation * KM_DEG_2_RAD(float)), M.Scaling(1.f, 1.f, 1.f)).Transpose();
					rotation += 25.f * dt;

					ID3D12DescriptorHeap* heaps[] =
					{
						textureDescriptor.Get(),
						samplerDescriptor.Get()
					};

					cmdList->SetDescriptorHeaps(_countof(heaps), heaps);

					cmdList->SetGraphicsRoot32BitConstants(0, 32, &constants, 0);
					
					cmdList->SetGraphicsRootDescriptorTable(1, textureDescriptor->GetGPUDescriptorHandleForHeapStart());

					cmdList->SetGraphicsRootDescriptorTable(2, samplerDescriptor->GetGPUDescriptorHandleForHeapStart());

					cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
					cmdList->IASetVertexBuffers(0, 1, &vertexBufferView);
					cmdList->IASetIndexBuffer(&indexBufferView);
					cmdList->DrawIndexedInstanced(static_cast<UINT>(indexCount), 1, 0, 0, 0);

					// Last barrier
					{
						const D3D12_RESOURCE_BARRIER barrier{
							.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION,
							.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE,
							.Transition = {
								.pResource = currentImage.Get(),
								.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES,
								.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET,
								.StateAfter = D3D12_RESOURCE_STATE_PRESENT,
							},
						};

						cmdList->ResourceBarrier(1, &barrier);
					}

					// Command Submit
					{
						cmdList->Close();
						ID3D12CommandList* cmdLists[] = { cmdList.Get() };
						queue->ExecuteCommandLists(_countof(cmdLists), cmdLists);
					}
				}

				// SwapChain Synchro
				{
					swapChain->Present(1, 0);

					const UINT cFence = scValues[swapchainFrame];
					queue->Signal(scFence.Get(), cFence);
				}
			}
		}
	}
}