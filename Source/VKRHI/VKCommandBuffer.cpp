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

void VKCommandBuffer::Close() const
{
	vkEndCommandBuffer(m_CommandBuffer);
}