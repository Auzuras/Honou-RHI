#include <DX12RHI/DX12CommandBuffer.h>

#include <DX12RHI/DX12Instance.h>
#include <DX12RHI/DX12Device.h>
#include <DX12RHI/DX12Buffer.h>
#include <DX12RHI/DX12Image.h>
#include <DX12RHI/DX12Pipeline.h>
//#include <DX12RHI/DX12Descriptor.h>

//VkPipelineBindPoint ToVkPipelineBindPoint(IPipeline::PipelineType _pipelineType)
//{
//	switch (_pipelineType)
//	{
//	default:
//	case IPipeline::PipelineType::HOUNOU_PIPELINE_TYPE_GRAPHICS:	return VK_PIPELINE_BIND_POINT_GRAPHICS;
//	case IPipeline::PipelineType::HOUNOU_PIPELINE_TYPE_COMPUTE:		return VK_PIPELINE_BIND_POINT_COMPUTE;
//	}
//}
//
D3D12_RESOURCE_STATES ToDX12Layout(ICommandBuffer::Layout _layout)
{
	switch (_layout)
	{
	default:
	case ICommandBuffer::Layout::UNDEFINED:					return D3D12_RESOURCE_STATE_COMMON;
	case ICommandBuffer::Layout::COLOR_OPTIMAL:				return D3D12_RESOURCE_STATE_RENDER_TARGET;
	case ICommandBuffer::Layout::DEPTH_OPTIMAL:				return D3D12_RESOURCE_STATE_DEPTH_WRITE;
	case ICommandBuffer::Layout::DEPTH_STENCIL_OPTIMAL:		return D3D12_RESOURCE_STATE_DEPTH_WRITE;
	case ICommandBuffer::Layout::SHADER_READ:				return D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
	case ICommandBuffer::Layout::UNORDERED_ACCESS:			return D3D12_RESOURCE_STATE_UNORDERED_ACCESS;
	case ICommandBuffer::Layout::TRANSFER_SRC:				return D3D12_RESOURCE_STATE_COPY_SOURCE;
	case ICommandBuffer::Layout::TRANSFER_DST:				return D3D12_RESOURCE_STATE_COPY_DEST;
	case ICommandBuffer::Layout::PRESENT:					return D3D12_RESOURCE_STATE_PRESENT;
	}
}
//
//VkPipelineStageFlags2 ToVkPipelineStage(uint64_t _stage)
//{
//	VkPipelineStageFlags2 flags = 0;
//
//	if ((_stage & (uint64_t)ICommandBuffer::PipelineStage::VERTEX_SHADER) != 0)
//		flags |= VK_PIPELINE_STAGE_2_VERTEX_SHADER_BIT;
//	if ((_stage & (uint64_t)ICommandBuffer::PipelineStage::FRAGMENT_SHADER) != 0)
//		flags |= VK_PIPELINE_STAGE_2_FRAGMENT_SHADER_BIT;
//	if ((_stage & (uint64_t)ICommandBuffer::PipelineStage::COMPUTE_SHADER) != 0)
//		flags |= VK_PIPELINE_STAGE_2_COMPUTE_SHADER_BIT;
//	if ((_stage & (uint64_t)ICommandBuffer::PipelineStage::EARLY_FRAGMENT_TEST) != 0)
//		flags |= VK_PIPELINE_STAGE_2_EARLY_FRAGMENT_TESTS_BIT;
//	if ((_stage & (uint64_t)ICommandBuffer::PipelineStage::LATE_FRAGMENT_TEST) != 0)
//		flags |= VK_PIPELINE_STAGE_2_LATE_FRAGMENT_TESTS_BIT;
//	if ((_stage & (uint64_t)ICommandBuffer::PipelineStage::COLOR_ATTACHMENT) != 0)
//		flags |= VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT;
//	if ((_stage & (uint64_t)ICommandBuffer::PipelineStage::TRANSFER) != 0)
//		flags |= VK_PIPELINE_STAGE_2_TRANSFER_BIT;
//
//	return flags;
//}
//
//VkAccessFlagBits2 ToVkAccess(uint64_t _layout)
//{
//	VkAccessFlagBits2 flags = 0;
//
//	if ((_layout & (uint64_t)ICommandBuffer::Access::SHADER_READ) != 0)
//		flags |= VK_ACCESS_2_SHADER_READ_BIT;
//	if ((_layout & (uint64_t)ICommandBuffer::Access::SHADER_WRITE) != 0)
//		flags |= VK_ACCESS_2_SHADER_WRITE_BIT;
//	if ((_layout & (uint64_t)ICommandBuffer::Access::COLOR_ATTACHMENT_READ) != 0)
//		flags |= VK_ACCESS_2_COLOR_ATTACHMENT_READ_BIT;
//	if ((_layout & (uint64_t)ICommandBuffer::Access::COLOR_ATTACHMENT_WRITE) != 0)
//		flags |= VK_ACCESS_2_COLOR_ATTACHMENT_WRITE_BIT;
//	if ((_layout & (uint64_t)ICommandBuffer::Access::DEPTH_STENCIL_ATTACHMENT_READ) != 0)
//		flags |= VK_ACCESS_2_DEPTH_STENCIL_ATTACHMENT_READ_BIT;
//	if ((_layout & (uint64_t)ICommandBuffer::Access::DEPTH_STENCIL_ATTACHMENT_WRITE) != 0)
//		flags |= VK_ACCESS_2_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
//	if ((_layout & (uint64_t)ICommandBuffer::Access::TRANSFER_READ) != 0)
//		flags |= VK_ACCESS_2_TRANSFER_READ_BIT;
//	if ((_layout & (uint64_t)ICommandBuffer::Access::TRANSFER_WRITE) != 0)
//		flags |= VK_ACCESS_2_TRANSFER_WRITE_BIT;
//
//	return flags;
//}

void DX12CommandBuffer::Open() const
{
	m_CommandBuffer->Reset(nullptr, nullptr);
}

void DX12CommandBuffer::Close() const
{
	m_CommandBuffer->Close();
}

void DX12CommandBuffer::Reset() const
{
	m_CommandBuffer->Reset(nullptr, nullptr);
}

void DX12CommandBuffer::BeginRendering(IImage* _image, IImage* _depth, uint32_t _width, uint32_t _height) const
{

}

void DX12CommandBuffer::EndRendering() const
{
	// No equivalent
}

void DX12CommandBuffer::BindPipeline(IPipeline* _pipeline) const
{
	ID3D12PipelineState* const pipeline = _pipeline->CastToDX12()->GetRaw();

	m_CommandBuffer->SetPipelineState(pipeline);
	m_CommandBuffer->SetGraphicsRootSignature(_pipeline->CastToDX12()->GetLayout());
}

void DX12CommandBuffer::SetViewport(float _width, float _height) const
{
	const D3D12_VIEWPORT viewport{
		.TopLeftX	=	0.f,
		.TopLeftY	=	0.f,
		.Width		=	_width,
		.Height		=	_height,
		.MinDepth	=	0.f,
		.MaxDepth	=	1.f
	};

	m_CommandBuffer->RSSetViewports(1, &viewport);
}

void DX12CommandBuffer::SetScissor(uint32_t _width, uint32_t _height) const
{
	const D3D12_RECT scissor{
		.left	=	0,
		.top	=	0,
		.right	=	static_cast<LONG>(_width),
		.bottom =	static_cast<LONG>(_height)
	};

	m_CommandBuffer->RSSetScissorRects(1, &scissor);
}

void DX12CommandBuffer::AddMemoryBarrier(IImage* _image, MemoryBarrierInfo _memoryBarrier) const
{
	ID3D12Resource* const image = _image->CastToDX12()->GetRaw();

	const D3D12_RESOURCE_BARRIER barrier{
		.Type	=	D3D12_RESOURCE_BARRIER_TYPE_TRANSITION,
		.Flags	=	D3D12_RESOURCE_BARRIER_FLAG_NONE,
		.Transition{
			.pResource		=	image,
			.Subresource	=	D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES,
			.StateBefore	=	ToDX12Layout(_memoryBarrier.oldLayout),
			.StateAfter		=	ToDX12Layout(_memoryBarrier.newLayout)
		}
	};

	m_CommandBuffer->ResourceBarrier(1, &barrier);
}

void DX12CommandBuffer::Copy(IImage* _destination, IImage* _source) const
{
	ID3D12Resource* const dst = _destination->CastToDX12()->GetRaw();
	ID3D12Resource* const src = _source->CastToDX12()->GetRaw();

	const D3D12_TEXTURE_COPY_LOCATION dstLocation{
		.pResource			=	dst,
		.Type				=	D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX,
		.SubresourceIndex	=	0
	};

	const D3D12_TEXTURE_COPY_LOCATION srcLocation{
		.pResource			=	src,
		.Type				=	D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX,
		.SubresourceIndex	=	0
	};

	const D3D12_BOX sourceBox{
		0, 
		0, 
		0,
		std::min(_source->GetWidth(), _destination->GetWidth()),
		std::min(_source->GetHeight(), _destination->GetHeight()),
		1
	};

	m_CommandBuffer->CopyTextureRegion(&dstLocation, 0, 0, 0, &srcLocation, &sourceBox);
}

void DX12CommandBuffer::Copy(IImage* _destination, IBuffer* _source) const
{

}

void DX12CommandBuffer::Copy(IBuffer* _destination, IImage* _source) const
{

}

void DX12CommandBuffer::Copy(IBuffer* _destination, IBuffer* _source) const
{
	ID3D12Resource* const dst = _destination->CastToDX12()->GetRaw();
	ID3D12Resource* const src = _source->CastToDX12()->GetRaw();

	const UINT64 size = std::min(0, 0);

	m_CommandBuffer->CopyBufferRegion(dst, 0, src, 0, size);
}

void DX12CommandBuffer::BindVertexBuffer(IBuffer* const _vertexBuffer) const
{
	//m_CommandBuffer->IASetVertexBuffers(0, 1, );
}

void DX12CommandBuffer::BindIndexBuffer(IBuffer* const _indexBuffer) const
{
	//m_CommandBuffer->IASetIndexBuffer();
}

void DX12CommandBuffer::BindConstants(IPipeline* _pipeline, uint32_t _offset, uint32_t _size, void* _data) const
{

}

void DX12CommandBuffer::BindDescriptor(IPipeline* _pipeline, IDescriptor* _descriptor, uint32_t _set) const
{

}

void DX12CommandBuffer::Draw(uint32_t _vertexCount) const
{
	m_CommandBuffer->DrawInstanced(_vertexCount, 1, 0, 0);
}

void DX12CommandBuffer::DrawIndexed(uint32_t _indexCount) const
{
	m_CommandBuffer->DrawIndexedInstanced(_indexCount, 1, 0, 0, 0);
}

void DX12CommandBuffer::Dispatch(uint32_t _groupCountX, uint32_t _groupCountY, uint32_t _groupCountZ) const
{
	m_CommandBuffer->Dispatch(_groupCountX, _groupCountY, _groupCountZ);
}

void DX12CommandBuffer::DrawIndexedIndirect(IBuffer* _buffer, uint32_t _offset) const
{

}

void DX12CommandBuffer::DispatchIndirect(IBuffer* _buffer, uint32_t _offset) const
{

}

void DX12CommandBuffer::AddLabel(const char* _name, const std::array<float, 4>& _color) const
{
	m_CommandBuffer->BeginEvent(0, _name, static_cast<UINT>(std::strlen(_name) + 1));
}

void DX12CommandBuffer::EndLabel() const
{
	m_CommandBuffer->EndEvent();
}
