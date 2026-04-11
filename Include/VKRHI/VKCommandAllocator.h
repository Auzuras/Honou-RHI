#ifndef HONOU_RENDER_VKCOMMANDALLOCATOR_H
#define HONOU_RENDER_VKCOMMANDALLOCATOR_H

class VKCommandAllocator;
class DX12CommandAllocator;
class OGLCommandAllocator;

#include <RHI/ICommandAllocator.h>

#include <vulkan/vulkan.h>

class VKCommandAllocator : public ICommandAllocator
{
public:
	~VKCommandAllocator() override final = default;

	void CreateCommandAllocator(IDevice* _device) override final;
	void DestroyCommandAllocator(IDevice* _device) override final;

	ICommandBuffer* AllocateCommandBuffers(IDevice* _device, uint32_t _count) override final;
	ICommandBuffer* AllocateCommandBuffer(IDevice* _device) override final;

	void FreeCommandBuffers(IDevice* _device, ICommandBuffer* _buffers, uint32_t _count) override final;
	void FreeCommandBuffer(IDevice* _device, ICommandBuffer* _buffer) override final;

	VKCommandAllocator* const CastToVK() { return this; }
	DX12CommandAllocator* CastToDX12() { return nullptr; }
	OGLCommandAllocator* CastToOGL() { return nullptr; }

private:
	VkCommandPool m_CommandAllocator = VK_NULL_HANDLE;
};

#endif //HONOU_RENDER_VKCOMMANDALLOCATOR_H
