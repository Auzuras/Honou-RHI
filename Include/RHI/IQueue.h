#ifndef HONOU_RENDER_IQUEUE_H
#define HONOU_RENDER_IQUEUE_H

class VKQueue;
class DX12Queue;
class OGLQueue;

class IDevice;
class ICommandBuffer;

class IQueue
{
public:
	virtual ~IQueue() = default;

	virtual void GetQueue(IDevice* _device) = 0;

	virtual void SubmitCommandBuffer(ICommandBuffer* _commandBuffer) = 0;

	virtual VKQueue* CastToVK() { return nullptr; }
	virtual DX12Queue* CastToDX12() { return nullptr; }
	virtual OGLQueue* CastToOGL() { return nullptr; }
};

#endif //HONOU_RENDER_VKQUEUE_H