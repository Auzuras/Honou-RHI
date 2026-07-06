#include <VKRHI/VKQueue.h>

#include <VKRHI/VKDevice.h>

#include <vector>

void VKQueue::GetQueue(IDevice* _device)
{
	const VkDevice lDevice = _device->CastToVK()->GetLogicalDevice();
	const VkPhysicalDevice pDevice = _device->CastToVK()->GetPhysicalDevice();

	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(pDevice, &queueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(pDevice, &queueFamilyCount, queueFamilies.data());

	int value = 0;

	const size_t familySize = queueFamilies.size();
	for (size_t i = 0; i < familySize; ++i)
	{
		const VkQueueFamilyProperties& queueFamily = queueFamilies[i];
		if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
			value = i;
	}

	vkGetDeviceQueue(lDevice, value, 0, &m_Queue);
}