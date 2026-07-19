#ifndef HONOU_RHI_VKINSTANCE_H
#define HONOU_RHI_VKINSTANCE_H

#include <RHI/IInstance.h>

#include <vulkan/vulkan.h>

namespace Hounou::Internal
{
	extern PFN_vkCmdBeginDebugUtilsLabelEXT vkCmdBeginDebugUtilsLabelEXT;
	extern PFN_vkCmdEndDebugUtilsLabelEXT vkCmdEndDebugUtilsLabelEXT;
	extern PFN_vkSetDebugUtilsObjectNameEXT vkSetDebugUtilsObjectNameEXT;
}

class VKInstance : public IInstance
{
public:
	~VKInstance() override final = default;

	void CreateInstance() override final;
	void DestroyInstance() override final;

	VKInstance* CastToVK() override final { return this; }
	DX12Instance* CastToDX12() override final { return nullptr; }
	OGLInstance* CastToOGL() override final { return nullptr; }

	VkInstance GetRaw() const { return m_Instance; }

private:

	static bool CheckValidationLayerSupport();

	static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT _messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT _messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* _pCallbackData,
		void* _pUserData
	);

	VkDebugUtilsMessengerEXT m_DebugMessenger = VK_NULL_HANDLE;
	VkInstance m_Instance = VK_NULL_HANDLE;
};

#endif //HONOU_RHI_VKINSTANCE_H