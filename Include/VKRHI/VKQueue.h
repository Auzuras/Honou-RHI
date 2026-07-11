#ifndef HONOU_RENDER_VKQUEUE_H
#define HONOU_RENDER_VKQUEUE_H

#include <RHI/IQueue.h>

#include <vulkan/vulkan.h>

class VKQueue : public IQueue
{
public:
	~VKQueue() override final = default;

	void GetQueue(IDevice* _device) override final = 0;

	void SubmitCommandBuffer(ICommandBuffer* _commandBuffer) override final;

	VKQueue* CastToVK() override final { return this; }
	DX12Queue* CastToDX12() override final { return nullptr; }
	OGLQueue* CastToOGL() override final { return nullptr; }

private:

	VkQueue m_Queue = VK_NULL_HANDLE;
};

#endif //HONOU_RENDER_VKPIPELINE_H