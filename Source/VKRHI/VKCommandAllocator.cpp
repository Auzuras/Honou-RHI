#include <VKRHI/VKCommandAllocator.h>

#include <VKRHI/VKDevice.h>
#include <VKRHI/VKCommandBuffer.h>

void VKCommandAllocator::CreateCommandAllocator(IDevice* _device)
{
	const VkDevice lDevice = _device->CastToVK()->GetLogicalDevice();
	const VkPhysicalDevice pDevice = _device->CastToVK()->GetPhysicalDevice();

	uint32_t queueFamilyCount{ 0 };
	vkGetPhysicalDeviceQueueFamilyProperties(pDevice, &queueFamilyCount, nullptr);
	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(pDevice, &queueFamilyCount, queueFamilies.data());

	uint32_t queueFamily{ 0 };
	for (size_t i = 0; i < queueFamilyCount; ++i)
	{
		if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
		{
			queueFamily = i;
			break;
		}
	}

	const VkCommandPoolCreateInfo commandPoolCI{
		.sType				=	VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
		.flags				=	VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
		.queueFamilyIndex	=	queueFamily
	};

	vkCreateCommandPool(lDevice, &commandPoolCI, nullptr, &m_CommandAllocator);
}

void VKCommandAllocator::DestroyCommandAllocator(IDevice* _device)
{
	VkDevice lDevice = _device->CastToVK()->GetLogicalDevice();
	vkDestroyCommandPool(lDevice, m_CommandAllocator, nullptr);
}

std::vector<ICommandBuffer*> VKCommandAllocator::AllocateCommandBuffers(IDevice* _device, uint32_t _count)
{
	const VkDevice lDevice = _device->CastToVK()->GetLogicalDevice();

	VkCommandBufferAllocateInfo cbAllocCI{
		.sType				=	VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
		.commandPool		=	m_CommandAllocator,
		.level				=	VK_COMMAND_BUFFER_LEVEL_PRIMARY,
		.commandBufferCount =	_count
	};

	std::vector<VkCommandBuffer> vkCmdBuffers{ _count };
	vkAllocateCommandBuffers(lDevice, &cbAllocCI, vkCmdBuffers.data());

	// Temp allocation for now
	std::vector<ICommandBuffer*> commandBuffers = std::vector<ICommandBuffer*>(_count);
	for (size_t i = 0; i < _count; ++i)
	{
		VKCommandBuffer* cmdBuffer = new VKCommandBuffer();
		cmdBuffer->m_CommandBuffer = vkCmdBuffers[i];
		commandBuffers[i] = cmdBuffer;
	}

	return commandBuffers;
}

ICommandBuffer* VKCommandAllocator::AllocateCommandBuffer(IDevice* _device)
{
	return AllocateCommandBuffers(_device, 1)[0];
}

void VKCommandAllocator::FreeCommandBuffers(IDevice* _device, const std::vector<ICommandBuffer*>& _buffers)
{
	for (size_t i = 0; i < _buffers.size(); ++i)
		FreeCommandBuffer(_device, _buffers[i]);
}

void VKCommandAllocator::FreeCommandBuffer(IDevice* _device, ICommandBuffer* _buffer)
{
	const VkDevice lDevice = _device->CastToVK()->GetLogicalDevice();
	vkFreeCommandBuffers(lDevice, m_CommandAllocator, 1, &_buffer->CastToVK()->m_CommandBuffer);
	delete _buffer;
}
