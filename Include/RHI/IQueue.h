#ifndef HONOU_RHI_IQUEUE_H
#define HONOU_RHI_IQUEUE_H

class VKQueue;
class DX12Queue;
class OGLQueue;

class IDevice;
class ICommandBuffer;
class IFence;
class ISyncPoint;

#include <cstdint>

class IQueue
{
public:

	enum class Type : uint8_t
	{
		QUEUE_TYPE_GRAPHICS		=	0,
		QUEUE_TYPE_COMPUTE		=	1,
		QUEUE_TYPE_COPY			=	2
	};

	virtual ~IQueue() = default;

	virtual void CreateQueue(IDevice* _device, Type _queueType) = 0;

	virtual void SubmitCommandBuffer(ICommandBuffer* _commandBuffer) = 0;
	virtual void SubmitCommandBuffer(ICommandBuffer* _commandBuffer, IFence* _fence, ISyncPoint* _waitPoint, ISyncPoint* _signalPoint) = 0;

	virtual void WaitIdle() = 0;

	virtual VKQueue* CastToVK() { return nullptr; }
	virtual DX12Queue* CastToDX12() { return nullptr; }
	virtual OGLQueue* CastToOGL() { return nullptr; }
};

#endif //HONOU_RHI_VKQUEUE_H