#ifndef HONOU_RENDER_VKDESCRIPTORALLOCATOR_H
#define HONOU_RENDER_VKDESCRIPTORALLOCATOR_H

class VKDescriptorAllocator;
class DX12DescriptorAllocator;
class OGLDescriptorAllocator;

#include <RHI/IDescriptorAllocator.h>

#include <vulkan/vulkan.h>

class VKDescriptorAllocator : public IDescriptorAllocator
{
public:
	~VKDescriptorAllocator() override final = default;

	void CreateDescriptorAllocator(IDevice* _device) override final;
	void DestroyDescriptorAllocator(IDevice* _device) override final;

	IDescriptor* AllocateDescriptors(IDevice* _device, uint32_t _count) override final;
	IDescriptor* AllocateDescriptor(IDevice* _device) override final;

	void FreeDescriptors(IDevice* _device, IDescriptor* _descriptors, uint32_t _count) override final;
	void FreeDescriptor(IDevice* _device, IDescriptor* _descriptor) override final;

	VKDescriptorAllocator* CastToVK() { return this; }
	DX12DescriptorAllocator* CastToDX12() { return nullptr; }
	OGLDescriptorAllocator* CastToOGL() { return nullptr; }

private:

	VkDescriptorPool m_DescriptorAllocator = VK_NULL_HANDLE;
};

#endif //HONOU_RENDER_VKDESCRIPTORALLOCATOR_H
