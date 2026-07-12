#ifndef HONOU_RENDER_IFENCE_H
#define HONOU_RENDER_IFENCE_H

class VKFence;
class DX12Fence;
class OGLFence;

class IDevice;

class IFence
{
public:
	virtual ~IFence() = default;

	virtual void CreateFence(IDevice* _device, bool _signaled = true) = 0;
	virtual void DestroyFence(IDevice* _device) = 0;

	virtual void Wait(IDevice* _device) = 0;
	virtual void Reset(IDevice* _device) = 0;

	bool IsSignaled() const { return p_IsSignaled; }

	virtual VKFence* CastToVK() { return nullptr; }
	virtual DX12Fence* CastToDX12() { return nullptr; }
	virtual OGLFence* CastToOGL() { return nullptr; }

protected:

	bool p_IsSignaled = false;
};

#endif //HONOU_RENDER_IFENCE_H