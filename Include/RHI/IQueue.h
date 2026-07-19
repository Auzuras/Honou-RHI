#ifndef HONOU_RHI_IQUEUE_H
#define HONOU_RHI_IQUEUE_H

class VKQueue;
class DX12Queue;
class OGLQueue;

class IDevice;
class ICommandBuffer;
class IFence;
class ISyncPoint;

class IQueue
{
public:
	virtual ~IQueue() = default;

	virtual void CreateQueue(IDevice* _device) = 0;

	virtual void SubmitCommandBuffer(ICommandBuffer* _commandBuffer) = 0;
	virtual void SubmitCommandBuffer(ICommandBuffer* _commandBuffer, IFence* _fence, ISyncPoint* _waitPoint, ISyncPoint* _signalPoint) = 0;

	virtual void WaitIdle() = 0;

	virtual VKQueue* CastToVK() { return nullptr; }
	virtual DX12Queue* CastToDX12() { return nullptr; }
	virtual OGLQueue* CastToOGL() { return nullptr; }
};

#endif //HONOU_RHI_VKQUEUE_H