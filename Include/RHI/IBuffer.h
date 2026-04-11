#ifndef HONOU_RENDER_IBUFFER_H
#define HONOU_RENDER_IBUFFER_H

class VKBuffer;
class DX12Buffer;
class OGLBuffer;

class IDevice;

class IBuffer
{
public:
	virtual ~IBuffer() = default;

	virtual void CreateBuffer(IDevice* _device) = 0;
	virtual void DestroyBuffer(IDevice* _device) = 0;

	virtual VKBuffer* const CastToVK() { return nullptr; }
	virtual DX12Buffer* CastToDX12() { return nullptr; }
	virtual OGLBuffer* CastToOGL() { return nullptr; }
};

#endif //HONOU_RENDER_IBUFFER_H
