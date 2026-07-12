#ifndef HONOU_RENDER_VKQUEUE_H
#define HONOU_RENDER_VKQUEUE_H

#include <RHI/IQueue.h>

#include <vulkan/vulkan.h>

class VKQueue : public IQueue
{
public:
	~VKQueue() override final = default;

	void CreateQueue(IDevice* _device) override final;

	void SubmitCommandBuffer(ICommandBuffer* _commandBuffer, IFence* _fence, ISyncPoint* _waitPoint, ISyncPoint* _signalPoint) override final;

	void WaitIdle() override final;

	VKQueue* CastToVK() override final { return this; }
	DX12Queue* CastToDX12() override final { return nullptr; }
	OGLQueue* CastToOGL() override final { return nullptr; }

	[[nordiscard]] VkQueue GetRaw() const { return m_Queue; }

private:

	VkQueue m_Queue = VK_NULL_HANDLE;
};

#endif //HONOU_RENDER_VKPIPELINE_H