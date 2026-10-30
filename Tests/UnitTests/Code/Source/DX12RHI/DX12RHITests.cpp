//#include <gtest/gtest.h>
//
//#include <Tests/Common.h>
//
//#include <DX12RHI/DX12Instance.h>
//#include <DX12RHI/DX12Device.h>
//#include <DX12RHI/DX12SwapChain.h>
//#include <DX12RHI/DX12Pipeline.h>
////#include <DX12RHI/DX12PipelineLayout.h>
//#include <DX12RHI/DX12CommandAllocator.h>
//#include <DX12RHI/DX12CommandBuffer.h>
//#include <DX12RHI/DX12Image.h>
//#include <DX12RHI/DX12Buffer.h>
//#include <DX12RHI/DX12ShaderModule.h>
////#include <DX12RHI/DX12SyncPoint.h>
//#include <DX12RHI/DX12Queue.h>
////#include <DX12RHI/DX12Sampler.h>
////#include <DX12RHI/DX12DescriptorAllocator.h>
////#include <DX12RHI/DX12Descriptor.h>
//
//class DX12RHITest : public testing::Test {};
////
////TEST(DX12RHITest, InstanceTest)
////{
////	DX12Instance instance;
////	instance.CreateInstance();
////	instance.DestroyInstance();
////}
////
////TEST(DX12RHITest, DeviceTest)
////{
////	DX12Instance instance;
////	instance.CreateInstance();
////
////	DX12Device device;
////	device.CreateDevice(&instance);
////	device.DestroyDevice();
////
////	instance.DestroyInstance();
////}
////
////TEST(DX12RHITest, SwapChainTest)
////{
////	DX12Instance instance;
////	instance.CreateInstance();
////
////	Window window(1920, 1080, "DX12 Window");
////
////	DX12Device device;
////	device.CreateDevice(&instance);
////
////	DX12Queue graphicsQueue;
////	graphicsQueue.CreateQueue(&device, IQueue::Type::QUEUE_TYPE_GRAPHICS);
////
////	DX12SwapChain swapChain;
////	const ISwapChain::Description scDesc{
////		.width = window.GetWidth(),
////		.height = window.GetHeight(),
////		.instance = GetModuleHandle(NULL),
////		.hwnd = window.GetHandle()
////	};
////	swapChain.CreateSwapChain(&instance, &device, scDesc);
////	swapChain.DestroySwapChain(&instance, &device);
////
////	device.DestroyDevice();
////	instance.DestroyInstance();
////
////	window.KooriDestroyWindow();
////}
////
////TEST(DX12RHITest, PipelineTest)
////{
////	DX12Instance instance;
////	instance.CreateInstance();
////
////	DX12Device device;
////	device.CreateDevice(&instance);
////
////	DX12ShaderModule vertSM;
////	vertSM.CreateShaderModule(&device, IShaderModule::GetShaderData("Assets/Shaders/Unlit/DX12/Unlit_VERT.cso"));
////
////	DX12ShaderModule pixSM;
////	pixSM.CreateShaderModule(&device, IShaderModule::GetShaderData("Assets/Shaders/Unlit/DX12/Unlit_PIX.cso"));
////
////	DX12Pipeline pipeline;
////	const IPipeline::GraphicsDescription gPipeline{
////		.vertexShader = {
////			.shader = &vertSM
////		},
////		.fragmentShader = {
////			.shader = &pixSM
////		}
////	};
////	pipeline.CreatePipeline(&device, gPipeline);
////	pipeline.DestroyPipeline(&device);
////
////	pixSM.DestroyShaderModule(&device);
////	vertSM.DestroyShaderModule(&device);
////
////	device.DestroyDevice();
////	instance.DestroyInstance();
////}
////
////TEST(DX12RHITest, CommandAllocatorTest)
////{
////	DX12Instance instance;
////	instance.CreateInstance();
////
////	DX12Device device;
////	device.CreateDevice(&instance);
////
////	DX12CommandAllocator cmdAlloc;
////	cmdAlloc.CreateCommandAllocator(&device);
////
////	ICommandBuffer* cmdBuffer = cmdAlloc.AllocateCommandBuffer(&device);
////	cmdAlloc.FreeCommandBuffer(&device, cmdBuffer);
////
////	cmdAlloc.DestroyCommandAllocator(&device);
////
////	device.DestroyDevice();
////	instance.DestroyInstance();
////}
//
////TEST(DX12RHITest, Mesh)
////{
////	tinyobj::attrib_t attrib{};
////	std::vector<tinyobj::shape_t> shapes{};
////	std::vector<tinyobj::material_t> materials{};
////	tinyobj::LoadObj(&attrib, &shapes, &materials, nullptr, nullptr, "Assets/Meshes/Knight.obj");
////
////	std::vector<Vertex> vertices{};
////	std::vector<uint16_t> indices{};
////
////	for (const tinyobj::index_t& index : shapes[0].mesh.indices)
////	{
////		const Vertex v{
////			.position = { attrib.vertices[index.vertex_index * 3], attrib.vertices[index.vertex_index * 3 + 1], attrib.vertices[index.vertex_index * 3 + 2] },
////			.normal = { attrib.normals[index.normal_index * 3], attrib.normals[index.normal_index * 3 + 1], attrib.normals[index.normal_index * 3 + 2] },
////			.textCoords = { attrib.texcoords[index.texcoord_index * 2], attrib.texcoords[index.texcoord_index * 2 + 1] }
////		};
////
////		vertices.push_back(v);
////		indices.push_back(static_cast<uint16_t>(indices.size()));
////	}
////
////	DX12Instance instance;
////	instance.CreateInstance();
////
////	DX12Device device;
////	device.CreateDevice(&instance);
////
////	const IBuffer::Description vertexDesc{
////		.usage = IBuffer::Usage::USAGE_VERTEX_BUFFER,
////		.size = sizeof(Vertex) * vertices.size(),
////		.stride = sizeof(Vertex),
////		.data = vertices.data()
////	};
////
////	DX12Buffer vertexBuffer;
////	vertexBuffer.CreateBuffer(&device, vertexDesc);
////	vertexBuffer.SetName(&device, "Knight Vertex Buffer");
////	vertexBuffer.DestroyBuffer(&device);
////
////	const IBuffer::Description indexDesc{
////		.usage = IBuffer::Usage::USAGE_INDEX_BUFFER,
////		.size = sizeof(uint16_t) * indices.size(),
////		.stride = sizeof(uint16_t),
////		.data = indices.data()
////	};
////
////	DX12Buffer indexBuffer;
////	indexBuffer.CreateBuffer(&device, indexDesc);
////	indexBuffer.SetName(&device, "Knight Index Buffer");
////	indexBuffer.DestroyBuffer(&device);
////
////	device.DestroyDevice();
////	instance.DestroyInstance();
////}
//
////TEST(DX12RHITest, Texture)
////{
////	int width, height, channels;
////	unsigned char* data = stbi_load("Assets/Textures/Knight.png", &width, &height, &channels, STBI_rgb_alpha);
////
////	if (!data) return;
////
////	DX12Instance instance;
////	instance.CreateInstance();
////
////	DX12Device device;
////	device.CreateDevice(&instance);
////
////	DX12Queue graphicsQueue;
////	graphicsQueue.CreateQueue(&device, IQueue::Type::QUEUE_TYPE_GRAPHICS);
////
////	DX12CommandAllocator cmdAlloc;
////	cmdAlloc.CreateCommandAllocator(&device);
////
////	ICommandBuffer* cmdBuffer = cmdAlloc.AllocateCommandBuffer(&device);
////
////	const IImage::Description imgDesc{
////		.width = static_cast<uint32_t>(width),
////		.height = static_cast<uint32_t>(height),
////		.channelCount = static_cast<uint8_t>(channels),
////		.format = IImage::Format::HONOU_FORMAT_R8G8B8A8_UNORM,
////		.usage = IImage::Usage((uint64_t)IImage::Usage::HOUNOU_USAGE_COLOR_ATTACHMENT | (uint64_t)IImage::Usage::HOUNOU_USAGE_SAMPLED | (uint64_t)IImage::Usage::HOUNOU_USAGE_TRANSFER_DST)
////	};
////
////	DX12Image texture;
////	texture.CreateImage(&device, imgDesc);
////
////	const IBuffer::Description stagingDesc{
////		.usage = IBuffer::Usage::USAGE_TRANSFER_SRC,
////		.size = static_cast<size_t>(width * height * 4),
////		.data = data
////	};
////
////	DX12Buffer stagingBuffer;
////	stagingBuffer.CreateBuffer(&device, stagingDesc);
////
////	cmdBuffer->Open();
////
////	const ICommandBuffer::MemoryBarrierInfo info{
////		.oldLayout = ICommandBuffer::Layout::UNDEFINED,
////		.newLayout = ICommandBuffer::Layout::TRANSFER_DST,
////
////		.syncDest{
////			.stages = ICommandBuffer::PipelineStage::TRANSFER,
////			.access = ICommandBuffer::Access::TRANSFER_WRITE
////		}
////	};
////
////	cmdBuffer->AddMemoryBarrier(&texture, info);
////
////	cmdBuffer->Copy(&texture, &stagingBuffer);
////
////	const ICommandBuffer::MemoryBarrierInfo info2{
////		.oldLayout = ICommandBuffer::Layout::TRANSFER_DST,
////		.newLayout = ICommandBuffer::Layout::SHADER_READ,
////
////		.syncSource{
////			.stages = ICommandBuffer::PipelineStage::TRANSFER,
////			.access = ICommandBuffer::Access::TRANSFER_WRITE
////		},
////
////		.syncDest{
////			.stages = ICommandBuffer::PipelineStage::FRAGMENT_SHADER,
////			.access = ICommandBuffer::Access::SHADER_READ
////		}
////	};
////
////	cmdBuffer->AddMemoryBarrier(&texture, info2);
////
////	cmdBuffer->Close();
////
////	device.GetGraphicsQueue()->SubmitCommandBuffer(cmdBuffer);
////
////	device.GetGraphicsQueue()->WaitIdle();
////
////	stagingBuffer.DestroyBuffer(&device);
////
////	texture.DestroyImage(&device);
////
////	stbi_image_free(data);
////
////	cmdAlloc.FreeCommandBuffer(&device, cmdBuffer);
////
////	cmdAlloc.DestroyCommandAllocator(&device);
////
////	device.DestroyDevice();
////	instance.DestroyInstance();
////}
//
//static void DebugCallback(D3D12_MESSAGE_CATEGORY _category, D3D12_MESSAGE_SEVERITY _severity, D3D12_MESSAGE_ID _id, LPCSTR _description, void* _context)
//{
//	(void*)_category, _id, _context;
//
//	switch (_severity)
//	{
//	case D3D12_MESSAGE_SEVERITY_CORRUPTION:
//	case D3D12_MESSAGE_SEVERITY_ERROR:
//	case D3D12_MESSAGE_SEVERITY_WARNING:
//	case D3D12_MESSAGE_SEVERITY_MESSAGE:
//		std::cout << _severity << " " << _description << "\n\n";
//		break;
//	}
//}
//
//static std::vector<char> GetShaderData(const std::string& _path)
//{
//	std::ifstream file(_path, std::ios::ate | std::ios::binary);
//
//	if (!file.is_open())
//		return std::vector<char>();
//
//	size_t fileSize = (size_t)file.tellg();
//	std::vector<char> buffer(fileSize);
//
//	file.seekg(0);
//	file.read(buffer.data(), fileSize);
//	file.close();
//
//	return buffer;
//}
//
//void DeviceWaitIdle(ID3D12CommandQueue* _queue, ID3D12Fence* _fence, uint32_t& _value, HANDLE _event)
//{
//	_queue->Signal(_fence, _value);
//
//	_fence->SetEventOnCompletion(_value, _event);
//	WaitForSingleObjectEx(_event, INFINITE, false);
//
//	++_value;
//};
//
//TEST(DX12RHITest, GraphicsRenderLoop)
//{
//	Window window(1920, 1080, "Raw DX12 Window");
//
//	const uint32_t MAX_FRAMES_IN_FLIGHT = 3;
//	const DXGI_FORMAT scFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
//	const DXGI_FORMAT depthFormat = DXGI_FORMAT_D32_FLOAT;
//
//	// Instance
//	DX12Instance instance;
//	instance.CreateInstance();
//
//	// Device
//	DX12Device device;
//	device.CreateDevice(&instance);
//
//	// Queue
//	DX12Queue queue;
//	queue.CreateQueue(&device, IQueue::Type::QUEUE_TYPE_GRAPHICS);
//
//	// Device Sync
//	HANDLE deviceEvent;
//	Microsoft::WRL::ComPtr<ID3D12Fence> deviceFence;
//	uint32_t deviceValue = 1u;
//	{
//		deviceEvent = CreateEvent(nullptr, false, false, nullptr);
//		device.GetRaw()->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&deviceFence));
//	}
//
//	const ISwapChain::Description scDesc{
//		.width = window.GetWidth(),
//		.height = window.GetHeight(),
//		.hwnd = window.GetHandle()
//	};
//
//	// SwapChain
//	DX12SwapChain swapChain;
//	swapChain.CreateSwapChain(&instance, &device, scDesc)
//
//	// SwapChain synchro
//	//HANDLE scEvent{};
//	//MPtr<ID3D12Fence> scFence{};
//	//std::array<uint32_t, MAX_FRAMES_IN_FLIGHT> scValues{};
//	//{
//	//	scEvent = CreateEvent(nullptr, false, false, nullptr);
//
//	//	device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&scFence));
//	//}
//
//	// Render targets
//	MPtr<ID3D12DescriptorHeap> scRTVHeap{};
//	DX12Image depthBuffer;
//	MPtr<ID3D12DescriptorHeap> sdepthRTVHeap{};
//	{
//		// SwapChain images
//		{
//			const D3D12_DESCRIPTOR_HEAP_DESC descriptorDesc{
//				.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV,
//				.NumDescriptors = MAX_FRAMES_IN_FLIGHT,
//				.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE,
//				.NodeMask = 0,
//			};
//
//			device->CreateDescriptorHeap(&descriptorDesc, IID_PPV_ARGS(&scRTVHeap));
//
//			D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = scRTVHeap->GetCPUDescriptorHandleForHeapStart();
//			const UINT rtvOffset = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
//
//			for (uint32_t i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i)
//			{
//				device->CreateRenderTargetView(scImages[i].Get(), nullptr, rtvHandle);
//				rtvHandle.ptr += rtvOffset;
//			}
//		}
//
//		// Depth
//		{
//			IImage::Description depth{
//				.width = window.GetWidth(),
//				.height = window.GetHeight(),
//				.format = IImage::Format::HOUNOU_FORMAT_D32_FLOAT_S8_UINT,
//				.usage = IImage::Usage((uint64_t)IImage::Usage::HOUNOU_USAGE_DEPTH_STENCIL_ATTACHMENT)
//			};
//
//			depthBuffer.CreateImage(&device, depth);
//
//			const D3D12_DESCRIPTOR_HEAP_DESC descriptorDesc{
//				.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV,
//				.NumDescriptors = 1,
//				.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE,
//				.NodeMask = 0,
//			};
//
//			device->CreateDescriptorHeap(&descriptorDesc, IID_PPV_ARGS(&sdepthRTVHeap));
//			device->CreateDepthStencilView(scDepth.Get(), nullptr, sdepthRTVHeap->GetCPUDescriptorHandleForHeapStart());
//		}
//	}
//
//	// Commands
//	std::array<MPtr<ID3D12CommandAllocator>, MAX_FRAMES_IN_FLIGHT> cmdAllocs{};
//	MPtr<ID3D12GraphicsCommandList1> cmdList{};
//	{
//		for (uint32_t i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i)
//		{
//			device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&cmdAllocs[i]));
//		}
//
//		device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, cmdAllocs[0].Get(), nullptr, IID_PPV_ARGS(&cmdList));
//		cmdList->Close();
//	}
//
//	// Buffers (Memory shared, require more work to be in only GPU)
//	MPtr<ID3D12Resource> vertexBuffer{};
//	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};
//
//	MPtr<ID3D12Resource> indexBuffer{};
//	D3D12_INDEX_BUFFER_VIEW indexBufferView{};
//
//	size_t indexCount = 0;
//	{
//		// Loading
//		tinyobj::attrib_t attrib{};
//		std::vector<tinyobj::shape_t> shapes{};
//		std::vector<tinyobj::material_t> materials{};
//		tinyobj::LoadObj(&attrib, &shapes, &materials, nullptr, nullptr, "Assets/Meshes/Knight.obj");
//
//		std::vector<Vertex> vertices{};
//		std::vector<uint16_t> indices{};
//
//		for (const tinyobj::index_t& index : shapes[0].mesh.indices)
//		{
//			const Vertex v{
//				.position = { attrib.vertices[index.vertex_index * 3], attrib.vertices[index.vertex_index * 3 + 1], attrib.vertices[index.vertex_index * 3 + 2] },
//				.normal = { attrib.normals[index.normal_index * 3], attrib.normals[index.normal_index * 3 + 1], attrib.normals[index.normal_index * 3 + 2] },
//				.textCoords = { attrib.texcoords[index.texcoord_index * 2], attrib.texcoords[index.texcoord_index * 2 + 1] }
//			};
//
//			vertices.push_back(v);
//			indices.push_back(static_cast<uint16_t>(indices.size()));
//		}
//
//		indexCount = indices.size();
//
//		// Heap can be UPLOAD (CPU -> GPU) | DEFAULT (GPU) | READBACK (GPU -> CPU)
//		const D3D12_HEAP_PROPERTIES heap{
//			.Type = D3D12_HEAP_TYPE_UPLOAD,
//		};
//
//		const D3D12_RESOURCE_DESC vertexDesc{
//			.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER,
//			.Alignment = 0,
//			.Width = vertices.size() * sizeof(Vertex),
//			.Height = 1,
//			.DepthOrArraySize = 1,
//			.MipLevels = 1,
//			.Format = DXGI_FORMAT_UNKNOWN,
//			.SampleDesc = { 1, 0 },
//			.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR,
//			.Flags = D3D12_RESOURCE_FLAG_NONE,
//		};
//
//		device->CreateCommittedResource(&heap, D3D12_HEAP_FLAG_NONE, &vertexDesc, D3D12_RESOURCE_STATE_COMMON, nullptr, IID_PPV_ARGS(&vertexBuffer));
//
//		// Upload
//		{
//			D3D12_RANGE mapRange{ 0, 0 };
//			void* data{};
//			vertexBuffer->Map(0, &mapRange, &data);
//			std::memcpy(data, vertices.data(), vertices.size() * sizeof(Vertex));
//			vertexBuffer->Unmap(0, nullptr);
//		}
//
//		vertexBufferView = D3D12_VERTEX_BUFFER_VIEW{
//			.BufferLocation = vertexBuffer->GetGPUVirtualAddress(),
//			.SizeInBytes = static_cast<UINT>(vertexDesc.Width),
//			.StrideInBytes = sizeof(Vertex)
//		};
//
//		const D3D12_RESOURCE_DESC indexDesc{
//			.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER,
//			.Alignment = 0,
//			.Width = sizeof(uint16_t) * indices.size(),
//			.Height = 1,
//			.DepthOrArraySize = 1,
//			.MipLevels = 1,
//			.Format = DXGI_FORMAT_UNKNOWN,
//			.SampleDesc = { 1, 0 },
//			.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR,
//			.Flags = D3D12_RESOURCE_FLAG_NONE,
//		};
//
//		device->CreateCommittedResource(&heap, D3D12_HEAP_FLAG_NONE, &indexDesc, D3D12_RESOURCE_STATE_COMMON, nullptr, IID_PPV_ARGS(&indexBuffer));
//
//		// Upload
//		{
//			D3D12_RANGE mapRange{ 0, 0 };
//			void* data{};
//			indexBuffer->Map(0, &mapRange, &data);
//			std::memcpy(data, indices.data(), indices.size() * sizeof(uint16_t));
//			indexBuffer->Unmap(0, nullptr);
//		}
//
//		indexBufferView = D3D12_INDEX_BUFFER_VIEW{
//			.BufferLocation = indexBuffer->GetGPUVirtualAddress(),
//			.SizeInBytes = static_cast<UINT>(indexDesc.Width),
//			.Format = DXGI_FORMAT_R16_UINT
//		};
//	}
//
//	cmdList->Reset(cmdAllocs[0].Get(), nullptr);
//
//	// Texture
//	MPtr<ID3D12Resource> texture{};
//	{
//		stbi_set_flip_vertically_on_load(true);
//
//		int width, height, channels;
//		unsigned char* textData = stbi_load("Assets/Textures/Knight.png", &width, &height, &channels, STBI_rgb_alpha);
//
//		// Heap can be UPLOAD (CPU -> GPU) | DEFAULT (GPU) | READBACK (GPU -> CPU)
//		const D3D12_HEAP_PROPERTIES heap{
//			.Type = D3D12_HEAP_TYPE_DEFAULT,
//		};
//
//		const D3D12_RESOURCE_DESC textureDesc{
//			.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D,
//			.Alignment = 0,
//			.Width = static_cast<UINT64>(width),
//			.Height = static_cast<UINT64>(height),
//			.DepthOrArraySize = 1,
//			.MipLevels = 1,
//			.Format = DXGI_FORMAT_R8G8B8A8_UNORM,
//			.SampleDesc = { 1, 0 },
//			.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN,
//			.Flags = D3D12_RESOURCE_FLAG_NONE,
//		};
//
//		device->CreateCommittedResource(&heap, D3D12_HEAP_FLAG_NONE, &textureDesc, D3D12_RESOURCE_STATE_COPY_DEST, nullptr, IID_PPV_ARGS(&texture));
//
//		// Upload
//		{
//			MPtr<ID3D12Resource> stagingBuffer{};
//
//			const D3D12_HEAP_PROPERTIES stagingHeap{
//				.Type = D3D12_HEAP_TYPE_UPLOAD,
//			};
//
//			const D3D12_RESOURCE_DESC stagingDesc{
//				.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER,
//				.Alignment = 0,
//				.Width = static_cast<UINT64>(width * height * channels),
//				.Height = 1,
//				.DepthOrArraySize = 1,
//				.MipLevels = 1,
//				.Format = DXGI_FORMAT_UNKNOWN,
//				.SampleDesc = { .Count = 1, .Quality = 0 },
//				.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR,
//				.Flags = D3D12_RESOURCE_FLAG_NONE,
//			};
//
//			device->CreateCommittedResource(&stagingHeap, D3D12_HEAP_FLAG_NONE, &stagingDesc, D3D12_RESOURCE_STATE_COPY_SOURCE, nullptr, IID_PPV_ARGS(&stagingBuffer));
//
//			D3D12_RANGE mapRange{ 0, 0 };
//			void* data{};
//			stagingBuffer->Map(0, &mapRange, &data);
//			std::memcpy(data, textData, width * height * channels);
//			stagingBuffer->Unmap(0, nullptr);
//
//			const D3D12_RESOURCE_DESC textDesc = texture->GetDesc();
//
//			const D3D12_TEXTURE_COPY_LOCATION src{
//				.pResource = stagingBuffer.Get(),
//				.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT,
//				.PlacedFootprint{
//					.Offset = 0u,
//					.Footprint{
//						.Format = textDesc.Format,
//						.Width = static_cast<UINT>(width),
//						.Height = static_cast<UINT>(height),
//						.Depth = 1,
//						.RowPitch = static_cast<UINT>(width * channels),
//					}
//				}
//			};
//
//			const D3D12_TEXTURE_COPY_LOCATION dst{
//				.pResource = texture.Get(),
//				.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX,
//				.SubresourceIndex = 0
//			};
//
//			cmdList->CopyTextureRegion(&dst, 0, 0, 0, &src, nullptr);
//
//			const D3D12_RESOURCE_BARRIER barrier{
//				.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION,
//				.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE,
//				.Transition = {
//					.pResource = texture.Get(),
//					.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES,
//					.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST,
//					.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
//				},
//			};
//
//			cmdList->ResourceBarrier(1, &barrier);
//
//			cmdList->Close();
//
//			ID3D12CommandList* cmdLists[] = { cmdList.Get() };
//			queue->ExecuteCommandLists(1, cmdLists);
//
//			DeviceWaitIdle(queue.Get(), deviceFence.Get(), deviceValue, deviceEvent);
//		}
//	}
//
//	// Descriptor heaps
//	MPtr<ID3D12DescriptorHeap> textureDescriptor{};
//	MPtr<ID3D12DescriptorHeap> samplerDescriptor{};
//	{
//		const D3D12_DESCRIPTOR_HEAP_DESC texDesc{
//			.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV,
//			.NumDescriptors = 1,
//			.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE
//		};
//
//		device->CreateDescriptorHeap(&texDesc, IID_PPV_ARGS(&textureDescriptor));
//
//		const D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{
//			.Format = DXGI_FORMAT_R8G8B8A8_UNORM,
//			.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D,
//			.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING,
//			.Texture2D{
//				.MipLevels = 1
//			}
//		};
//
//		device->CreateShaderResourceView(texture.Get(), &srvDesc, textureDescriptor->GetCPUDescriptorHandleForHeapStart());
//
//		const D3D12_DESCRIPTOR_HEAP_DESC samplerDescriptorDesc{
//			.Type = D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER,
//			.NumDescriptors = 1,
//			.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE
//		};
//
//		device->CreateDescriptorHeap(&samplerDescriptorDesc, IID_PPV_ARGS(&samplerDescriptor));
//
//		const D3D12_SAMPLER_DESC samplerDesc{
//			.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR,
//			.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP,
//			.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP,
//			.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP,
//			.MipLODBias = 0.0f,
//			.MaxAnisotropy = 1,
//			.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER,
//			.BorderColor = { 0.f, 0.f, 0.f, 0.f },
//			.MinLOD = 0.0f,
//			.MaxLOD = D3D12_FLOAT32_MAX
//		};
//
//		device->CreateSampler(&samplerDesc, samplerDescriptor->GetCPUDescriptorHandleForHeapStart());
//	}
//
//	// Loop
//	{
//		float rotation = 0.f;
//		auto lastTime = std::chrono::high_resolution_clock::now();
//
//		while (!window.WindowShouldClose())
//		{
//			auto currentTime = std::chrono::high_resolution_clock::now();
//			float dt = std::chrono::duration<float>(currentTime - lastTime).count();
//			lastTime = currentTime;
//
//			// Update
//			{
//				window.UpdateWindow();
//			}
//
//			// Render
//			{
//				// SwapChain Synchro
//				{
//					const UINT pFence = scValues[swapchainFrame];
//					swapchainFrame = swapChain->GetCurrentBackBufferIndex();
//					const UINT cFence = scValues[swapchainFrame];
//
//					if (scFence->GetCompletedValue() < cFence)
//					{
//						scFence->SetEventOnCompletion(cFence, scEvent);
//						WaitForSingleObjectEx(scEvent, INFINITE, FALSE);
//					}
//
//					scValues[swapchainFrame] = pFence + 1;
//				}
//
//				// Draw
//				{
//					// Command Setup
//					{
//						MPtr<ID3D12CommandAllocator> cmdAlloc = cmdAllocs[swapchainFrame];
//						cmdAlloc->Reset();
//						cmdList->Reset(cmdAlloc.Get(), nullptr);
//					}
//
//					MPtr<ID3D12Resource> currentImage = scImages[swapchainFrame];
//
//					// First barrier
//					{
//						const D3D12_RESOURCE_BARRIER barrier{
//							.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION,
//							.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE,
//							.Transition = {
//								.pResource = currentImage.Get(),
//								.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES,
//								.StateBefore = D3D12_RESOURCE_STATE_COMMON,
//								.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET,
//							},
//						};
//
//						cmdList->ResourceBarrier(1, &barrier);
//					}
//
//
//					D3D12_CPU_DESCRIPTOR_HANDLE scImageHandle = scRTVHeap->GetCPUDescriptorHandleForHeapStart();
//					const UINT rtvOffset = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
//					scImageHandle.ptr += rtvOffset * swapchainFrame;
//
//					const D3D12_CPU_DESCRIPTOR_HANDLE depthHandle = sdepthRTVHeap->GetCPUDescriptorHandleForHeapStart();
//
//					float clearColor[] = { 0.0f, 0.0f, 0.2f, 1.0f };
//
//					cmdList->OMSetRenderTargets(1, &scImageHandle, FALSE, &depthHandle);
//					cmdList->ClearRenderTargetView(scImageHandle, clearColor, 0, nullptr);
//					cmdList->ClearDepthStencilView(depthHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
//
//					const D3D12_VIEWPORT viewport{
//						.TopLeftX = 0,
//						.TopLeftY = 0,
//						.Width = float(window.GetWidth()),
//						.Height = float(window.GetHeight()),
//						.MinDepth = 0.0f,
//						.MaxDepth = 1.0f,
//					};
//
//					const D3D12_RECT scissor{
//						.left = 0,
//						.top = 0,
//						.right = static_cast<LONG>(window.GetWidth()),
//						.bottom = static_cast<LONG>(window.GetHeight()),
//					};
//
//					cmdList->RSSetViewports(1, &viewport);
//					cmdList->RSSetScissorRects(1, &scissor);
//
//					cmdList->SetGraphicsRootSignature(rootSignature.Get());
//
//					cmdList->SetPipelineState(pipelineState.Get());
//
//					Matrix4f M;
//					Matrix4f VP;
//
//					struct
//					{
//						Matrix4f Model;
//						Matrix4f ProjectionView;
//					} constants;
//
//					const Matrix4f V = VP.ViewMatrix(
//						Vector3f(0.f, 0.f, 0.f),
//						Vector3f(0.f, 0.f, 1.f),
//						Vector3f(0.f, 1.f, 0.f)
//					);
//
//					const Matrix4f P = VP.PerspectiveProjection(
//						0.1f,
//						100.f,
//						float(window.GetWidth()) / float(window.GetHeight()),
//						90.f * KM_DEG_2_RAD(float)
//					);
//
//					constants.ProjectionView = (P * V);
//					constants.Model = M.TRS(M.Translation(0.f, -2.5f, -5.f), M.YRotation(rotation * KM_DEG_2_RAD(float)), M.Scaling(1.f, 1.f, 1.f)).Transpose();
//					rotation += 25.f * dt;
//
//					ID3D12DescriptorHeap* heaps[] =
//					{
//						textureDescriptor.Get(),
//						samplerDescriptor.Get()
//					};
//
//					cmdList->SetDescriptorHeaps(_countof(heaps), heaps);
//
//					cmdList->SetGraphicsRoot32BitConstants(0, 32, &constants, 0);
//
//					cmdList->SetGraphicsRootDescriptorTable(1, textureDescriptor->GetGPUDescriptorHandleForHeapStart());
//
//					cmdList->SetGraphicsRootDescriptorTable(2, samplerDescriptor->GetGPUDescriptorHandleForHeapStart());
//
//					cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
//					cmdList->IASetVertexBuffers(0, 1, &vertexBufferView);
//					cmdList->IASetIndexBuffer(&indexBufferView);
//					cmdList->DrawIndexedInstanced(static_cast<UINT>(indexCount), 1, 0, 0, 0);
//
//					// Last barrier
//					{
//						const D3D12_RESOURCE_BARRIER barrier{
//							.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION,
//							.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE,
//							.Transition = {
//								.pResource = currentImage.Get(),
//								.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES,
//								.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET,
//								.StateAfter = D3D12_RESOURCE_STATE_PRESENT,
//							},
//						};
//
//						cmdList->ResourceBarrier(1, &barrier);
//					}
//
//					// Command Submit
//					{
//						cmdList->Close();
//						ID3D12CommandList* cmdLists[] = { cmdList.Get() };
//						queue->ExecuteCommandLists(_countof(cmdLists), cmdLists);
//					}
//				}
//
//				// SwapChain Synchro
//				{
//					swapChain->Present(1, 0);
//
//					const UINT cFence = scValues[swapchainFrame];
//					queue->Signal(scFence.Get(), cFence);
//				}
//			}
//		}
//	}
//}
