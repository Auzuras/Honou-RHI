#include <VKRHI/VKFence.h>

#include <VKRHI/VKDevice.h>

void VKFence::CreateFence(IDevice* _device)
{
	const VkDevice lDevice = _device->CastToVK()->GetLogicalDevice();

	const VkSemaphoreTypeCreateInfo semaphoreTypeCI{
		.sType = VK_STRUCTURE_TYPE_SEMAPHORE_TYPE_CREATE_INFO,
		.semaphoreType = VK_SEMAPHORE_TYPE_TIMELINE,
		.initialValue = 0
	};

	const VkSemaphoreCreateInfo semaphoreCI{
		.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
		.pNext = &semaphoreTypeCI
	};

	vkCreateSemaphore(lDevice, &semaphoreCI, nullptr, &m_Semaphore);
}

void VKFence::DestroyFence(IDevice* _device)
{
	const VkDevice lDevice = _device->CastToVK()->GetLogicalDevice();
	vkDestroySemaphore(lDevice, m_Semaphore, nullptr);
}

void VKFence::SignalGPU(IDevice* _device, uint64_t _value)
{
	const VkDevice lDevice = _device->CastToVK()->GetLogicalDevice();

	const VkSemaphoreSignalInfo semaphoreSignalInfo{
		.sType = VK_STRUCTURE_TYPE_SEMAPHORE_SIGNAL_INFO,
		.semaphore = m_Semaphore, 
		.value = _value
	};

	vkSignalSemaphore(lDevice, &semaphoreSignalInfo);
}

void VKFence::WaitCPU(IDevice* _device, uint64_t _value)
{
	const VkDevice lDevice = _device->CastToVK()->GetLogicalDevice();

	uint64_t completedValue = 0;
	vkGetSemaphoreCounterValue(lDevice, m_Semaphore, &completedValue);

	if (completedValue >= _value)
		return;

	const VkSemaphoreWaitInfo semaphoreWaitInfo{
		.sType = VK_STRUCTURE_TYPE_SEMAPHORE_WAIT_INFO,
		.semaphoreCount = 1, 
		.pSemaphores = &m_Semaphore,
		.pValues = &_value
	};

	const VkResult result = vkWaitSemaphores(lDevice, &semaphoreWaitInfo, UINT64_MAX);
}