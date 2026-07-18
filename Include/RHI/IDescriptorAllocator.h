#ifndef HONOU_RENDER_IDESCRIPTORALLOCATOR_H
#define HONOU_RENDER_IDESCRIPTORALLOCATOR_H

#include <cstdint>

#include <vector>

class VKDescriptorAllocator;
class DX12DescriptorAllocator;
class OGLDescriptorAllocator;

class IDevice;
class IPipeline;
class IDescriptor;

class IDescriptorAllocator
{
public:
	virtual ~IDescriptorAllocator() = default;

	virtual void CreateDescriptorAllocator(IDevice* _device) = 0;
	virtual void DestroyDescriptorAllocator(IDevice* _device) = 0;

	virtual std::vector<IDescriptor*> AllocateDescriptors(IDevice* _device, IPipeline* _pipeline, uint32_t _count) = 0;
	virtual IDescriptor* AllocateDescriptor(IDevice* _device, IPipeline* _pipeline) = 0;

	virtual void FreeDescriptors(IDevice* _device, const std::vector<IDescriptor*>& _descriptors) = 0;
	virtual void FreeDescriptor(IDevice* _device, IDescriptor* _descriptor) = 0;

	virtual VKDescriptorAllocator* CastToVK() { return nullptr; }
	virtual DX12DescriptorAllocator* CastToDX12() { return nullptr; }
	virtual OGLDescriptorAllocator* CastToOGL() { return nullptr; }
};

#endif //HONOU_RENDER_IDESCRIPTORALLOCATOR_H
