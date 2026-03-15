#ifndef HONOU_RENDER_IDEVICE_H
#define HONOU_RENDER_IDEVICE_H

class VKDevice;
class DX12Device;
class OGLDevice;

class IInstance;

class IDevice
{
public:
	virtual ~IDevice() { };

	virtual void CreateDevice(IInstance* _instance) = 0;
	virtual void DestroyDevice() = 0;

	virtual VKDevice* CastToVK() { return nullptr; }
	virtual DX12Device* CastToDX12() { return nullptr; }
	virtual OGLDevice* CastToOGL() { return nullptr; }
};

#endif //HONOU_RENDER_IDEVICE_H