#include <VKRHI/VKShaderModule.h>

#include <VKRHI/VKDevice.h>

void VKShaderModule::CreateShaderModule(IDevice* _device, const std::vector<char>& _data)
{
	const VkDevice ldevice = _device->CastToVK()->GetLogicalDevice();

	VkShaderModuleCreateInfo shaderModuleCI{
		.sType		=	VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
		.codeSize	=	_data.size(),
		.pCode		=	reinterpret_cast<const uint32_t*>(_data.data())
	};

	vkCreateShaderModule(ldevice, &shaderModuleCI, nullptr, &m_ShaderModule);
}

void VKShaderModule::DestroyShaderModule(IDevice* _device)
{
	const VkDevice ldevice = _device->CastToVK()->GetLogicalDevice();
	vkDestroyShaderModule(ldevice, m_ShaderModule, nullptr);
}