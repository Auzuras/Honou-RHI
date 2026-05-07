#include <VKRHI/VKInstance.h>

#include <vector>
#include <iostream>

void VKInstance::CreateInstance()
{
	VkApplicationInfo appInfo {
		.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
		.pApplicationName = "Vulkan",
		.apiVersion = VK_API_VERSION_1_3
	};
	
	const bool enableVL = CheckValidationLayerSupport();
	const std::vector<const char*> validationLayers = { "VK_LAYER_KHRONOS_validation" };
	
	std::vector<const char*> extensions;

	if (enableVL)
		extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		extensions.push_back("VK_KHR_surface");
		extensions.push_back("VK_KHR_win32_surface");

	VkInstanceCreateInfo instanceDesc{
		.sType						=	VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
		.pApplicationInfo			=	&appInfo,
		.enabledLayerCount			=	enableVL ? static_cast<uint32_t>(validationLayers.size()) : 0u,
		.ppEnabledLayerNames		=	enableVL ? validationLayers.data() : nullptr,
		.enabledExtensionCount		=	enableVL ? static_cast<uint32_t>(extensions.size()) : 0u,
		.ppEnabledExtensionNames	=	enableVL ? extensions.data() : nullptr
	};

	vkCreateInstance(&instanceDesc, nullptr, &m_Instance);

	VkDebugUtilsMessengerCreateInfoEXT debugMessengerCI{
		.sType				=	VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,

		.messageSeverity	=	VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT 
							|	VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT 
							|	VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,

		.messageType		=	VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT 
							|	VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT 
							|	VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,

		.pfnUserCallback	=	DebugCallback,
		.pUserData			=	nullptr
	};

	auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(m_Instance, "vkCreateDebugUtilsMessengerEXT");
	if (func) func(m_Instance, &debugMessengerCI, nullptr, &m_DebugMessenger);
}

void VKInstance::DestroyInstance()
{
	auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(m_Instance, "vkDestroyDebugUtilsMessengerEXT");
	if (func) func(m_Instance, m_DebugMessenger, nullptr);

	vkDestroyInstance(m_Instance, nullptr);
}

bool VKInstance::CheckValidationLayerSupport()
{
	uint32_t layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

	std::vector<VkLayerProperties> availableLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

	const std::vector<const char*> validationLayers = { "VK_LAYER_KHRONOS_validation" };

	for (const char* layerName : validationLayers)
	{
		bool layerFound = false;

		for (const VkLayerProperties& layerProperties : availableLayers)
		{
			if (strcmp(layerName, layerProperties.layerName) == 0)
			{
				layerFound = true;
				break;
			}
		}

		if (!layerFound)
			return false;
	}

	return true;
}

VKAPI_ATTR VkBool32 VKAPI_CALL VKInstance::DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT _messageSeverity, VkDebugUtilsMessageTypeFlagsEXT _messageType, const VkDebugUtilsMessengerCallbackDataEXT* _callBackData, void* _userData)
{
	_userData;

	switch (_messageSeverity)
	{
	default: case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT: case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
		std::cout << "INFO: " << _messageType << " Vulkan Validation Layer : " << _callBackData->pMessage << "\n\n";
		break;
	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
		std::cout << "WARN: " << _messageType << " Vulkan Validation Layer: " << _callBackData->pMessage << "\n\n";
		break;
	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT: case VK_DEBUG_UTILS_MESSAGE_SEVERITY_FLAG_BITS_MAX_ENUM_EXT:
		std::cout << "ERROR: " << _messageType << " Vulkan Validation Layer: " << _callBackData->pMessage << "\n\n";
		break;
	}

	return VK_FALSE;
}
