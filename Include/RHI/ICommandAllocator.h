#ifndef HONOU_RENDER_ICOMMANDALLOCATOR_H
#define HONOU_RENDER_ICOMMANDALLOCATOR_H

#include <cstdint>
#include <vector>

class VKCommandAllocator;
class DX12CommandAllocator;
class OGLCommandAllocator;

class IDevice;
class ICommandBuffer;

class ICommandAllocator
{
public:
	virtual ~ICommandAllocator() = default;

	virtual void CreateCommandAllocator(IDevice* _device) = 0;
	virtual void DestroyCommandAllocator(IDevice* _device) = 0;

	virtual std::vector<ICommandBuffer*> AllocateCommandBuffers(IDevice* _device, uint32_t _count) = 0;
	virtual ICommandBuffer* AllocateCommandBuffer(IDevice* _device) = 0;

	virtual void FreeCommandBuffers(IDevice* _device, const std::vector<ICommandBuffer*>& _buffers) = 0;
	virtual void FreeCommandBuffer(IDevice* _device, ICommandBuffer* _buffer) = 0;

	virtual VKCommandAllocator* const CastToVK() { return nullptr; }
	virtual DX12CommandAllocator* CastToDX12() { return nullptr; }
	virtual OGLCommandAllocator* CastToOGL() { return nullptr; }
};

#endif //HONOU_RENDER_ICOMMANDALLOCATOR_H
