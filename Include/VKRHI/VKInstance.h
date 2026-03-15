#ifndef HONOU_RENDER_VKINSTANCE_H
#define HONOU_RENDER_VKINSTANCE_H

#include <RHI/IInstance.h>

#include <vulkan/vulkan.h>

class VKInstance : public IInstance
{
public:
	~VKInstance() override final { };

	void CreateInstance() override final;
	void DestroyInstance() override final;

	VKInstance* CastToVK() override final { return this; }
	DX12Instance* CastToDX12() override final { return nullptr; }
	OGLInstance* CastToOGL() override final { return nullptr; }

	VkInstance GetRaw() const { return m_Instance; }

private:
	VkInstance m_Instance = VK_NULL_HANDLE;
};

#endif //HONOU_RENDER_VKINSTANCE_H