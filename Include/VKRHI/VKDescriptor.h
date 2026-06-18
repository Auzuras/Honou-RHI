#ifndef HONOU_RENDER_VKDESCRIPTOR_H
#define HONOU_RENDER_VKDESCRIPTOR_H

#include <RHI/IDescriptor.h>

#include <vulkan/vulkan.h>

class VKDescriptor : public IDescriptor
{
public:
	~VKDescriptor() override final = default;

	VKDescriptor* CastToVK() override final { return this; }
	DX12Descriptor* CastToDX12() override final { return nullptr; }
	OGLDescriptor* CastToOGL() override final { return nullptr; }

private:
	VkDescriptorSet m_Descriptor = VK_NULL_HANDLE;
};

#endif //HONOU_RENDER_VKDESCRIPTOR_H