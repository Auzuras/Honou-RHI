#ifndef HONOU_RENDER_VKSHADERMODULE_H
#define HONOU_RENDER_VKSHADERMODULE_H

#include <RHI/IShaderModule.h>

#include <vulkan/vulkan.h>

class VKShaderModule : public IShaderModule
{
public:
	~VKShaderModule() override final = default;

	void CreateShaderModule(IDevice* _device, const std::vector<char>& _data) override final;
	void DestroyShaderModule(IDevice* _device) override final;

	VkShaderModule GetRaw() const { return m_ShaderModule; }

	VKShaderModule* const CastToVK() override final { return this; }
	DX12ShaderModule* CastToDX12() override final { return nullptr; }
	OGLShaderModule* CastToOGL() override final { return nullptr; }

private:

	VkShaderModule m_ShaderModule = VK_NULL_HANDLE;
};

#endif //HONOU_RENDER_VKSHADERMODULE_H
