#ifndef HONOU_RENDER_ICOMMANDBUFFER_H
#define HONOU_RENDER_ICOMMANDBUFFER_H

#include <cstdint>

class VKCommandBuffer;
class DX12CommandBuffer;
class OGLCommandBuffer;

class IDevice;

class ICommandBuffer
{
public:
	virtual ~ICommandBuffer() = default;

	virtual void Open() const;
	virtual void Close() const;

	virtual VKCommandBuffer* const CastToVK() { return nullptr; }
	virtual DX12CommandBuffer* CastToDX12() { return nullptr; }
	virtual OGLCommandBuffer* CastToOGL() { return nullptr; }
};

#endif //HONOU_RENDER_ICOMMANDBUFFER_H
