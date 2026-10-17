#include <DX12RHI/DX12Pipeline.h>

#include <DX12RHI/DX12Device.h>
#include <DX12RHI/DX12ShaderModule.h>

void DX12Pipeline::CreatePipeline(IDevice* _device, const GraphicsDescription& _pipelineDescription)
{
	ID3D12Device* const device = _device->CastToDX12()->GetRaw();

	const D3D12_VIEWPORT viewport{
		.TopLeftX	=	0.f,
		.TopLeftY	=	0.f,
		.Width		=	1920.f,
		.Height		=	1080.f,
		.MinDepth	=	0.f,
		.MaxDepth	=	1.f
	};

	const D3D12_RECT scissor{
		.left		=	0,
		.top		=	0,
		.right		=	1920,
		.bottom		=	1080
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

	const D3D12_ROOT_PARAMETER1 rootParams[]{
		{
			.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV,
			.Descriptor{
				.ShaderRegister = 0,
				.RegisterSpace = 0,
				.Flags = D3D12_ROOT_DESCRIPTOR_FLAG_DATA_STATIC_WHILE_SET_AT_EXECUTE
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
			.NumParameters		= _countof(rootParams),
			.pParameters		= rootParams,
			.NumStaticSamplers	= 0,
			.pStaticSamplers	= nullptr,
			.Flags				= D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT
		}
	};

	Microsoft::WRL::ComPtr<ID3D10Blob> signature;
	Microsoft::WRL::ComPtr<ID3D10Blob> error;

	HRESULT result = D3D12SerializeVersionedRootSignature(&rootDesc, &signature, &error);

	if (FAILED(result))
	{

	}

	result = device->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&m_PipelineLayout));

	if (FAILED(result))
	{

	}

	const D3D12_RENDER_TARGET_BLEND_DESC rtBlendDesc{
		.BlendEnable	=	false,
		.LogicOpEnable	=	false,

		.SrcBlend		=	D3D12_BLEND_ONE,
		.DestBlend		=	D3D12_BLEND_ZERO,
		.BlendOp		=	D3D12_BLEND_OP_ADD,

		.SrcBlendAlpha	=	D3D12_BLEND_ONE,
		.DestBlendAlpha =	D3D12_BLEND_ZERO,
		.BlendOpAlpha	=	D3D12_BLEND_OP_ADD,

		.LogicOp		=	D3D12_LOGIC_OP_NOOP,

		.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL
	};

	const D3D12_BLEND_DESC blendDesc{
		.AlphaToCoverageEnable		=	false,
		.IndependentBlendEnable		=	false,

		.RenderTarget{
			rtBlendDesc
		}
	};

	const D3D12_RASTERIZER_DESC rasterizerDesc{
		.FillMode				=	D3D12_FILL_MODE_SOLID,
		.CullMode				=	D3D12_CULL_MODE_BACK,
		.FrontCounterClockwise	=	FALSE,
		.DepthBias				=	D3D12_DEFAULT_DEPTH_BIAS,
		.DepthBiasClamp			=	D3D12_DEFAULT_DEPTH_BIAS_CLAMP,
		.SlopeScaledDepthBias	=	D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS,
		.DepthClipEnable		=	TRUE,
		.MultisampleEnable		=	FALSE,
		.AntialiasedLineEnable	=	FALSE,
		.ForcedSampleCount		=	0,
		.ConservativeRaster		=	D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF
	};

	const D3D12_DEPTH_STENCIL_DESC depthStencDesc{
		.DepthEnable		=	TRUE,
		.DepthWriteMask		=	D3D12_DEPTH_WRITE_MASK_ALL,
		.DepthFunc			=	D3D12_COMPARISON_FUNC_LESS_EQUAL,
		.StencilEnable		=	FALSE,
		.StencilReadMask	=	D3D12_DEFAULT_STENCIL_READ_MASK,
		.StencilWriteMask	=	D3D12_DEFAULT_STENCIL_WRITE_MASK,
		.FrontFace{
			.StencilFailOp		=	D3D12_STENCIL_OP_KEEP,
			.StencilDepthFailOp =	D3D12_STENCIL_OP_KEEP,
			.StencilPassOp		=	D3D12_STENCIL_OP_KEEP,
			.StencilFunc		=	D3D12_COMPARISON_FUNC_ALWAYS,
		},
		.BackFace{
			.StencilFailOp		=	D3D12_STENCIL_OP_KEEP,
			.StencilDepthFailOp =	D3D12_STENCIL_OP_KEEP,
			.StencilPassOp		=	D3D12_STENCIL_OP_KEEP,
			.StencilFunc		=	D3D12_COMPARISON_FUNC_ALWAYS,
		}
	};

	const D3D12_INPUT_ELEMENT_DESC inputDescs[]{
		{
			.SemanticName			=	"POSITION",
			.SemanticIndex			=	0,
			.Format					=	DXGI_FORMAT_R32G32B32_FLOAT,
			.InputSlot				=	0,
			.AlignedByteOffset		=	0,
			.InputSlotClass			=	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
			.InstanceDataStepRate	=	0
		},
		{
			.SemanticName			=	"NORMAL",
			.SemanticIndex			=	0,
			.Format					=	DXGI_FORMAT_R32G32B32_FLOAT,
			.InputSlot				=	1,
			.AlignedByteOffset		=	0,
			.InputSlotClass			=	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
			.InstanceDataStepRate	=	0
		},
		{
			.SemanticName			=	"TEXCOORD",
			.SemanticIndex			=	0,
			.Format					=	DXGI_FORMAT_R32G32_FLOAT,
			.InputSlot				=	3,
			.AlignedByteOffset		=	0,
			.InputSlotClass			=	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
			.InstanceDataStepRate	=	0
		}
	};

	const D3D12_INPUT_LAYOUT_DESC inputDesc{
		.pInputElementDescs		=	inputDescs,
		.NumElements			=	_countof(inputDescs)
	};

	ID3DBlob* const vertexShaderModule = _pipelineDescription.vertexShader.shader->CastToDX12()->GetRaw();
	ID3DBlob* const pixelShaderModule = _pipelineDescription.fragmentShader.shader->CastToDX12()->GetRaw();

	const D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineDesc{
		.pRootSignature = m_PipelineLayout.Get(),

		.VS{
			.pShaderBytecode = vertexShaderModule->GetBufferPointer(),
			.BytecodeLength = vertexShaderModule->GetBufferSize()
		},
		.PS{
			.pShaderBytecode = pixelShaderModule->GetBufferPointer(),
			.BytecodeLength = pixelShaderModule->GetBufferSize()
		},

		.StreamOutput{},
		.BlendState = blendDesc,
		.SampleMask = UINT_MAX,

		.RasterizerState = rasterizerDesc,
		.InputLayout = inputDesc,

		.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE,

		.NumRenderTargets = 1,
		.RTVFormats{
			DXGI_FORMAT_R8G8B8A8_UNORM
		},
		.DSVFormat = DXGI_FORMAT_D16_UNORM,

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

	result = device->CreateGraphicsPipelineState(&graphicsPipelineDesc, IID_PPV_ARGS(&m_Pipeline));

	if (FAILED(result))
	{

	}
}

void DX12Pipeline::CreatePipeline(IDevice* _device, const ComputeDescription& _pipelineDescription)
{
}

void DX12Pipeline::DestroyPipeline(IDevice* _Device)
{
	m_Pipeline = nullptr;
}