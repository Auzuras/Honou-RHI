#include <VKRHI/VKCommandBuffer.h>

#include <VKRHI/VKDevice.h>
#include <VKRHI/VKBuffer.h>
#include <VKRHI/VKImage.h>
#include <VKRHI/VKPipeline.h>

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

void VKCommandBuffer::BeginRendering(IImage* _image, uint32_t _width, uint32_t _height) const
{
	const VkImageView imageview = _image->CastToVK()->GetRawImageView();

	const VkRenderingAttachmentInfo colorAttachementInfo{
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
		.pColorAttachments = &colorAttachementInfo
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
	vkCmdBindPipeline(m_CommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
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

void VKCommandBuffer::AddMemoryBarrier0(IImage* _image) const
{
	VkImage image = _image->CastToVK()->GetRaw();

	const VkImageMemoryBarrier2 barrier{
		.sType			=	VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2,

		.srcStageMask	=	VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT,
		.srcAccessMask	=	0,

		.dstStageMask	=	VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT,
		.dstAccessMask	=	VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,

		.oldLayout		=	VK_IMAGE_LAYOUT_UNDEFINED,
		.newLayout		=	VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL,

		.image			=	image,

		.subresourceRange{
			.aspectMask		=	VK_IMAGE_ASPECT_COLOR_BIT,
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

void VKCommandBuffer::AddMemoryBarrier1(IImage* _image) const
{
	VkImage image = _image->CastToVK()->GetRaw();

	const VkImageMemoryBarrier2 barrier{
		.sType			=	VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2,

		.srcStageMask	=	VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT,
		.srcAccessMask	=	0,

		.dstStageMask	=	VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT,
		.dstAccessMask	=	VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,

		.oldLayout		=	VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL,
		.newLayout		=	VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,

		.image			=	image,

		.subresourceRange{
			.aspectMask		=	VK_IMAGE_ASPECT_COLOR_BIT,
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

	// TODO
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
		.imageExtent = { 0, 0, 1 }
	};

	vkCmdCopyBufferToImage(m_CommandBuffer, srcBuffer, destImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);
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
