#ifndef HONOU_RHI_IFENCE_H
#define HONOU_RHI_IFENCE_H

class VKFence;
class DX12Fence;
class OGLFence;

class IDevice;

#include <cstdint>

class IFence
{
public:
	virtual ~IFence() = default;

	virtual void CreateFence(IDevice* _device) = 0;
	virtual void DestroyFence(IDevice* _device) = 0;

	virtual void SignalGPU(IDevice* _device, uint64_t _value) = 0;
	virtual void WaitCPU(IDevice* _device, uint64_t _value) = 0;

	virtual VKFence* CastToVK() { return nullptr; }
	virtual DX12Fence* CastToDX12() { return nullptr; }
	virtual OGLFence* CastToOGL() { return nullptr; }

protected:

	uint64_t p_NextValue = 0ull;
};

struct SyncPoint
{
	IFence* fence = nullptr;
	uint64_t fenceValue = 0ull;
};

#endif //HONOU_RHI_IFENCE_H