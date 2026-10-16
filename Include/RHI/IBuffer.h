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

	enum class Usage : uint64_t
	{
		USAGE_NONE,
		USAGE_VERTEX_BUFFER = 0,
		USAGE_INDEX_BUFFER = 1,
		USAGE_TRANSFER_SRC = 2,
		USAGE_TRANSFER_DST = 3
	};

	struct Description
	{
		Usage usage		=	Usage::USAGE_NONE;
		size_t size		=	0;
	};

	virtual ~IBuffer() = default;

	virtual void CreateBuffer(IDevice* _device, const Description& _description) = 0;
	virtual void DestroyBuffer(IDevice* _device) = 0;

	virtual void SetName(IDevice* _device, const char* _name) = 0;

	virtual VKBuffer* const CastToVK() { return nullptr; }
	virtual DX12Buffer* CastToDX12() { return nullptr; }
	virtual OGLBuffer* CastToOGL() { return nullptr; }
};

#endif //HONOU_RHI_IBUFFER_H
