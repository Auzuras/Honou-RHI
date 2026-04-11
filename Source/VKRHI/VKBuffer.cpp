#include <VKRHI/VKBuffer.h>

#include <VKRHI/VKDevice.h>

void VKBuffer::CreateBuffer(IDevice* _device)
{
	VkDevice lDevice = _device->CastToVK()->GetLogicalDevice();
	
	size_t size = 0;

	VkBufferCreateInfo bufferCI{
		.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
		.size = size,
		.usage = 0
	};

	vkCreateBuffer(lDevice, &bufferCI, nullptr, &m_Buffer);
}

void VKBuffer::DestroyBuffer(IDevice* _device)
{
	VkDevice lDevice = _device->CastToVK()->GetLogicalDevice();
	vkDestroyBuffer(lDevice, m_Buffer, nullptr);
}