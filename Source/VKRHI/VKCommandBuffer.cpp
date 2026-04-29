#include <VKRHI/VKCommandBuffer.h>

#include <VKRHI/VKDevice.h>

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