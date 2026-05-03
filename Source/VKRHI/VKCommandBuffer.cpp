#include <VKRHI/VKCommandBuffer.h>

#include <VKRHI/VKDevice.h>
#include <VKRHI/VKBuffer.h>
#include <VKRHI/VKImage.h>

void VKCommandBuffer::Open() const
{
	VkCommandBufferBeginInfo cbBI{
		.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
		.flags = 0
	};

	vkBeginCommandBuffer(m_CommandBuffer, &cbBI);
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

void VKCommandBuffer::MemoryBarrier(IImage* _image) const
{
	VkImage image = _image->CastToVK()->GetRaw();

	const VkImageMemoryBarrier barrier{
		.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,

		.srcAccessMask = 0,
		.dstAccessMask = 0,

		.oldLayout = VK_IMAGE_LAYOUT_READ_ONLY_OPTIMAL,
		.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,

		.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
		.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,

		.image = image,

		.subresourceRange = {
			.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
			.baseMipLevel = 0,
			.levelCount = 1,
			.baseArrayLayer = 0,
			.layerCount = 1
		}
	};

	vkCmdPipelineBarrier(m_CommandBuffer, 0, 0, 0, 0, nullptr, 0, nullptr, 1, &barrier);
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

	vkCmdBindIndexBuffer(m_CommandBuffer, vkBuffer->GetRaw(), 0, VK_INDEX_TYPE_UINT32);
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

void VKCommandBuffer::Close() const
{
	vkEndCommandBuffer(m_CommandBuffer);
}