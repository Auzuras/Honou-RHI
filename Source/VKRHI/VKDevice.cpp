#include <VKRHI/VKDevice.h>

#include <VKRHI/VKInstance.h>

void VKDevice::CreateDevice(IInstance* _instance)
{
	const VkInstance instance = _instance->CastToVK()->GetRaw();

	uint32_t deviceCount{ 0 };
	vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

	const uint32_t selectedDevice = SelectBestDevice(devices);

	uint32_t queueFamilyCount{ 0 };
	vkGetPhysicalDeviceQueueFamilyProperties(devices[selectedDevice], &queueFamilyCount, nullptr);
	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(devices[selectedDevice], &queueFamilyCount, queueFamilies.data());

	uint32_t queueFamily{ 0 };
	for (size_t i = 0; i < queueFamilyCount; ++i)
	{
		if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
		{
			queueFamily = i;
			break;
		}
	}

	const float QFPriorities{ 1.0f };
	VkDeviceQueueCreateInfo queueCI{
		.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
		.queueFamilyIndex = queueFamily,
		.queueCount = 1,
		.pQueuePriorities = &QFPriorities
	};

	const std::vector<const char*> deviceExtensions{ VK_KHR_SWAPCHAIN_EXTENSION_NAME };

	VkPhysicalDeviceVulkan12Features enabledVk12Features{
		.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES,
		.descriptorIndexing = true,
		.shaderSampledImageArrayNonUniformIndexing = true,
		.descriptorBindingVariableDescriptorCount = true,
		.runtimeDescriptorArray = true,
		.bufferDeviceAddress = true
	};

	const VkPhysicalDeviceVulkan13Features enabledVk13Features{
		.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES,
		.pNext = &enabledVk12Features,
		.synchronization2 = true,
		.dynamicRendering = true
	};

	const VkPhysicalDeviceFeatures enabledVk10Features{
		.samplerAnisotropy = VK_TRUE
	};

	VkDeviceCreateInfo deviceCI{
		.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
		.pNext = &enabledVk13Features,
		.queueCreateInfoCount = 1,
		.pQueueCreateInfos = &queueCI,
		.enabledExtensionCount = 1,
		.ppEnabledExtensionNames = deviceExtensions.data(),
		.pEnabledFeatures = &enabledVk10Features
	};

	vkCreateDevice(devices[selectedDevice], &deviceCI, nullptr, &m_Device);
	vkGetDeviceQueue(m_Device, queueFamily, 0, &m_GraphicsQueue);
}

void VKDevice::DestroyDevice()
{
	vkDestroyDevice(m_Device, nullptr);
}

uint32_t VKDevice::SelectBestDevice(const std::vector<VkPhysicalDevice>& devices)
{
	// TODO: The method
	//const size_t deviceCount = devices.size();

	//VkPhysicalDeviceProperties2 deviceProperties{
	//	.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2,
	//};

	//for (size_t i = 0; i < deviceCount; ++i)
	//{
	//	vkGetPhysicalDeviceProperties2(devices[0], &deviceProperties);
	//}

	return 0;
}