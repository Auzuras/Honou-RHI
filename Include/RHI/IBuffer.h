#ifndef HONOU_RHI_IBUFFER_H
#define HONOU_RHI_IBUFFER_H

#include <cstdint>

class VKBuffer;
class DX12Buffer;
class OGLBuffer;

class IDevice;

class IBuffer
{
public:

	enum class BufferUsage : uint32_t
	{
		USAGE_VERTEX_BUFFER		=	0,
		USAGE_INDEX_BUFFER		=	1,
		USAGE_TRANSFER_SRC		=	2,
		USAGE_TRANSFER_DST		=	3
	};

	virtual ~IBuffer() = default;

	virtual void CreateBuffer(IDevice* _device, void* _data, size_t _size, BufferUsage _usage) = 0;
	virtual void DestroyBuffer(IDevice* _device) = 0;

	virtual VKBuffer* const CastToVK() { return nullptr; }
	virtual DX12Buffer* CastToDX12() { return nullptr; }
	virtual OGLBuffer* CastToOGL() { return nullptr; }
};

#endif //HONOU_RHI_IBUFFER_H
