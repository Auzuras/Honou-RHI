#include <VKRHI/VKInstance.h>

#include <vector>

void VKInstance::CreateInstance()
{
	VkApplicationInfo appInfo {
		.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
		.pApplicationName = "Vulkan",
		.apiVersion = VK_API_VERSION_1_3
	};

	uint32_t instanceExtensionCount{ 0 };
	
	VkInstanceCreateInfo instanceDesc{
		.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
		.pApplicationInfo = &appInfo,
		.enabledExtensionCount = instanceExtensionCount
	};

	vkCreateInstance(&instanceDesc, nullptr, &m_Instance);
}

void VKInstance::DestroyInstance()
{
	vkDestroyInstance(m_Instance, nullptr);
}