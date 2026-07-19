#ifndef HONOU_RHI_IDEVICE_H
#define HONOU_RHI_IDEVICE_H

class VKDevice;
class DX12Device;
class OGLDevice;

class IInstance;
class IQueue;

class IDevice
{
public:
	virtual ~IDevice() = default;

	virtual void CreateDevice(IInstance* _instance) = 0;
	virtual void DestroyDevice() = 0;

	virtual void WaitIdle() = 0;

	void SetGraphicsQueue(IQueue* _queue) { m_GraphicsQueueRef = _queue; }
	IQueue* GetGraphicsQueue() const { return m_GraphicsQueueRef; }

	virtual VKDevice* const CastToVK() { return nullptr; }
	virtual DX12Device* CastToDX12() { return nullptr; }
	virtual OGLDevice* CastToOGL() { return nullptr; }

private:

	IQueue* m_GraphicsQueueRef = nullptr;
};

#endif //HONOU_RHI_IDEVICE_H