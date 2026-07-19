#include <VKRHI/VKCommandBuffer.h>

#include <VKRHI/VKDevice.h>
#include <VKRHI/VKBuffer.h>
#include <VKRHI/VKImage.h>
#include <VKRHI/VKPipeline.h>
#include <VKRHI/VKDescriptor.h>

VkImageLayout ToVkLayout(ICommandBuffer::Layout _layout)
{
	switch (_layout)
	{
	default:
	case ICommandBuffer::Layout::UNDEFINED:					return VK_IMAGE_LAYOUT_UNDEFINED;
	case ICommandBuffer::Layout::COLOR_OPTIMAL:				return VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL;
	case ICommandBuffer::Layout::DEPTH_OPTIMAL:				return VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL;
	case ICommandBuffer::Layout::DEPTH_STENCIL_OPTIMAL:		return VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
	case ICommandBuffer::Layout::SHADER_READ:				return VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	case ICommandBuffer::Layout::UNORDERED_ACCESS:			return VK_IMAGE_LAYOUT_GENERAL;
	case ICommandBuffer::Layout::TRANSFER_SRC:				return VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
	case ICommandBuffer::Layout::TRANSFER_DST:				return VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
	case ICommandBuffer::Layout::PRESENT:					return VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
	}
}

VkPipelineStageFlags2 ToVkPipelineStage(uint64_t _stage)
{
	VkPipelineStageFlags2 flags = 0;

	if ((_stage & (uint64_t)ICommandBuffer::PipelineStage::VERTEX_SHADER) != 0)
		flags |= VK_PIPELINE_STAGE_2_VERTEX_SHADER_BIT;
	if ((_stage & (uint64_t)ICommandBuffer::PipelineStage::FRAGMENT_SHADER) != 0)
		flags |= VK_PIPELINE_STAGE_2_FRAGMENT_SHADER_BIT;
	if ((_stage & (uint64_t)ICommandBuffer::PipelineStage::COMPUTE_SHADER) != 0)
		flags |= VK_PIPELINE_STAGE_2_COMPUTE_SHADER_BIT;
	if ((_stage & (uint64_t)ICommandBuffer::PipelineStage::EARLY_FRAGMENT_TEST) != 0)
		flags |= VK_PIPELINE_STAGE_2_EARLY_FRAGMENT_TESTS_BIT;
	if ((_stage & (uint64_t)ICommandBuffer::PipelineStage::LATE_FRAGMENT_TEST) != 0)
		flags |= VK_PIPELINE_STAGE_2_LATE_FRAGMENT_TESTS_BIT;
	if ((_stage & (uint64_t)ICommandBuffer::PipelineStage::COLOR_ATTACHMENT) != 0)
		flags |= VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT;
	if ((_stage & (uint64_t)ICommandBuffer::PipelineStage::TRANSFER) != 0)
		flags |= VK_PIPELINE_STAGE_2_TRANSFER_BIT;

	return flags;
}

VkAccessFlagBits2 ToVkAccess(uint64_t _layout)
{
	VkAccessFlagBits2 flags = 0;

	if ((_layout & (uint64_t)ICommandBuffer::Access::SHADER_READ) != 0)
		flags |= VK_ACCESS_2_SHADER_READ_BIT;
	if ((_layout & (uint64_t)ICommandBuffer::Access::SHADER_WRITE) != 0)
		flags |= VK_ACCESS_2_SHADER_WRITE_BIT;
	if ((_layout & (uint64_t)ICommandBuffer::Access::COLOR_ATTACHMENT_READ) != 0)
		flags |= VK_ACCESS_2_COLOR_ATTACHMENT_READ_BIT;
	if ((_layout & (uint64_t)ICommandBuffer::Access::COLOR_ATTACHMENT_WRITE) != 0)
		flags |= VK_ACCESS_2_COLOR_ATTACHMENT_WRITE_BIT;
	if ((_layout & (uint64_t)ICommandBuffer::Access::DEPTH_STENCIL_ATTACHMENT_READ) != 0)
		flags |= VK_ACCESS_2_DEPTH_STENCIL_ATTACHMENT_READ_BIT;
	if ((_layout & (uint64_t)ICommandBuffer::Access::DEPTH_STENCIL_ATTACHMENT_WRITE) != 0)
		flags |= VK_ACCESS_2_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
	if ((_layout & (uint64_t)ICommandBuffer::Access::TRANSFER_READ) != 0)
		flags |= VK_ACCESS_2_TRANSFER_READ_BIT;
	if ((_layout & (uint64_t)ICommandBuffer::Access::TRANSFER_WRITE) != 0)
		flags |= VK_ACCESS_2_TRANSFER_WRITE_BIT;

	return flags;
}

void VKCommandBuffer::Open() const
{
	VkCommandBufferBeginInfo cbBI{
		.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
		.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT
	};

	vkBeginCommandBuffer(m_CommandBuffer, &cbBI);
}

void VKCommandBuffer::Close() const
{
	vkEndCommandBuffer(m_CommandBuffer);
}

void VKCommandBuffer::Reset() const
{
	vkResetCommandBuffer(m_CommandBuffer, 0);
}

void VKCommandBuffer::BeginRendering(IImage* _image, IImage* _depth, uint32_t _width, uint32_t _height) const
{
	const VkImageView imageview = _image->CastToVK()->GetRawImageView();
	const VkImageView depthview = _depth ? _depth->CastToVK()->GetRawImageView() : VK_NULL_HANDLE;

	const VkRenderingAttachmentInfo colorAttachmentInfo{
		.sType			=	VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO,
		.imageView		=	imageview,
		.imageLayout	=	VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL,
		.loadOp			=	VK_ATTACHMENT_LOAD_OP_CLEAR,
		.storeOp		=	VK_ATTACHMENT_STORE_OP_STORE,
		.clearValue{
			.color{ 
				0.0f, 0.0f, 0.2f, 1.0f 
			}
		}
	};

	const VkRenderingAttachmentInfo depthAttachmentInfo{
		.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO,
		.imageView = depthview,
		.imageLayout = VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL,
		.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
		.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
		.clearValue{
			.depthStencil{
				1.0f, 0
			}
		}
	};

	const VkRenderingInfo renderingInfo{
		.sType = VK_STRUCTURE_TYPE_RENDERING_INFO,
		.renderArea{
			.extent{
				.width = _width,
				.height = _height
			}
		},
		.layerCount = 1,
		.colorAttachmentCount = 1,
		.pColorAttachments = &colorAttachmentInfo,
		.pDepthAttachment = _depth ? &depthAttachmentInfo : VK_NULL_HANDLE,
	};

	vkCmdBeginRendering(m_CommandBuffer, &renderingInfo);
}

void VKCommandBuffer::EndRendering() const
{
	vkCmdEndRendering(m_CommandBuffer);
}

void VKCommandBuffer::BindPipeline(IPipeline* _pipeline) const
{
	VkPipeline pipeline = _pipeline->CastToVK()->GetRaw();
	vkCmdBindPipeline(m_CommandBuffer, VK_PIPELINE_BIND_POINT_COMPUTE, pipeline);
}

void VKCommandBuffer::SetViewport(float _width, float _height) const
{
	VkViewport viewport{
		.x			=	0.f,
		.y			=	0.f,
		.width		=	_width,
		.height		=	_height,
		.minDepth	=	0.f,
		.maxDepth	=	1.f
	};

	vkCmdSetViewport(m_CommandBuffer, 0, 1, &viewport);
}

void VKCommandBuffer::SetScissor(uint32_t _width, uint32_t _height) const
{
	VkExtent2D extent{
		.width		=	_width,
		.height		=	_height
	};

	VkRect2D scissor{
		.offset		=	{0, 0},
		.extent		=	extent
	};

	vkCmdSetScissor(m_CommandBuffer, 0, 1, &scissor);
}

void VKCommandBuffer::AddMemoryBarrier(IImage* _image, MemoryBarrierInfo _memoryBarrier) const
{
	VkImage image = _image->CastToVK()->GetRaw();

	const VkImageMemoryBarrier2 barrier{
		.sType			=	VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2,

		.srcStageMask	=	ToVkPipelineStage((uint64_t)_memoryBarrier.syncSource.stages),
		.srcAccessMask	=	ToVkAccess((uint64_t)_memoryBarrier.syncSource.access),

		.dstStageMask	=	ToVkPipelineStage((uint64_t)_memoryBarrier.syncDest.stages),
		.dstAccessMask	=	ToVkAccess((uint64_t)_memoryBarrier.syncDest.access),

		.oldLayout		=	ToVkLayout(_memoryBarrier.oldLayout),
		.newLayout		=	ToVkLayout(_memoryBarrier.newLayout),

		.image			=	image,

		.subresourceRange{
			.aspectMask		=  _image->GetFormat() == TextureFormat::HOUNOU_FORMAT_D32_FLOAT_S8_UINT ? (VkImageAspectFlags)(VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT) : (VkImageAspectFlags)VK_IMAGE_ASPECT_COLOR_BIT,
			.levelCount		=	1,
			.layerCount		=	1
		}
	};

	VkDependencyInfo barrierDependencyInfo{
		.sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO,
		.imageMemoryBarrierCount = 1,
		.pImageMemoryBarriers = &barrier
	};

	vkCmdPipelineBarrier2(m_CommandBuffer, &barrierDependencyInfo);
}

void VKCommandBuffer::Copy(IImage* _destination, IImage* _source) const
{
	VkImage destImage = _destination->CastToVK()->GetRaw();
	VkImage srcImage = _source->CastToVK()->GetRaw();

	const VkImageCopy region{
		.srcSubresource{
			.aspectMask		=	VK_IMAGE_ASPECT_COLOR_BIT,
			.mipLevel		=	0,
			.baseArrayLayer =	0,
			.layerCount		=	1
		},
		.srcOffset{
			0, 0, 0
		},

		.dstSubresource{
			.aspectMask		=	VK_IMAGE_ASPECT_COLOR_BIT,
			.mipLevel		=	0,
			.baseArrayLayer =	0,
			.layerCount		=	1
		},
		.dstOffset{
			0, 0, 0
		},
		.extent{
			.width	= std::min(_destination->GetWidth(), _source->GetWidth()),
			.height = std::min(_destination->GetHeight(), _source->GetHeight()),
			.depth  = 1
		}
	};

	vkCmdCopyImage(m_CommandBuffer, srcImage, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, destImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);
}

void VKCommandBuffer::Copy(IImage* _destination, IBuffer* _source) const
{
	VkImage destImage = _destination->CastToVK()->GetRaw();
	VkBuffer srcBuffer = _source->CastToVK()->GetRaw();

	const VkBufferImageCopy region{
		.bufferOffset = 0,
		.bufferRowLength = 0,
		.bufferImageHeight = 0,

		.imageSubresource = {
			.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
			.mipLevel = 0,
			.baseArrayLayer = 0,
			.layerCount = 1
		},

		.imageOffset = { 0, 0, 0 },
		.imageExtent = { _destination->GetWidth(), _destination->GetHeight(), 1}
	};

	vkCmdCopyBufferToImage(m_CommandBuffer, srcBuffer, destImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);
}

void VKCommandBuffer::Copy(IBuffer* _destination, IImage* _source) const
{
	// TODO
}

void VKCommandBuffer::Copy(IBuffer* _destination, IBuffer* _source) const
{
	// TODO
}

void VKCommandBuffer::BindVertexBuffer(IBuffer* const _vertexBuffer) const
{
	VKBuffer* const vkBuffer = _vertexBuffer->CastToVK();

	const VkBuffer vertexBuffers[] = { vkBuffer->GetRaw() };
	const VkDeviceSize offsets[] = { 0 };

	vkCmdBindVertexBuffers(m_CommandBuffer, 0, 1, vertexBuffers, offsets);
}

void VKCommandBuffer::BindIndexBuffer(IBuffer* const _indexBuffer) const
{
	VKBuffer* const vkBuffer = _indexBuffer->CastToVK();

	vkCmdBindIndexBuffer(m_CommandBuffer, vkBuffer->GetRaw(), 0, VK_INDEX_TYPE_UINT16);
}

void VKCommandBuffer::BindConstants(IPipeline* _pipeline, uint32_t _offset, uint32_t _size, void* _data) const
{
	const VkPipelineLayout pipelineLayout = _pipeline->CastToVK()->GetLayout();
	vkCmdPushConstants(m_CommandBuffer, pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, _offset, _size, _data);
}

void VKCommandBuffer::BindDescriptor(IPipeline* _pipeline, IDescriptor* _descriptor, uint32_t _set) const
{
	const VkPipelineLayout pipelineLayout = _pipeline->CastToVK()->GetLayout();
	const VkDescriptorSet descriptor = _descriptor->CastToVK()->GetRaw();
	vkCmdBindDescriptorSets(m_CommandBuffer, VK_PIPELINE_BIND_POINT_COMPUTE, pipelineLayout, _set, 1, &descriptor, 0, nullptr);
}

void VKCommandBuffer::Draw(uint32_t _vertexCount) const
{
	vkCmdDraw(m_CommandBuffer, _vertexCount, 1, 0, 0);
}

void VKCommandBuffer::DrawIndexed(uint32_t _indexCount) const
{
	vkCmdDrawIndexed(m_CommandBuffer, _indexCount, 1, 0, 0, 0);
}

void VKCommandBuffer::Dispatch(uint32_t _groupCountX, uint32_t _groupCountY, uint32_t _groupCountZ) const
{
	vkCmdDispatch(m_CommandBuffer, _groupCountX, _groupCountY, _groupCountZ);
}
