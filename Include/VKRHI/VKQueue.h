#ifndef HONOU_RHI_VKQUEUE_H
#define HONOU_RHI_VKQUEUE_H

#include <RHI/IQueue.h>

#include <vulkan/vulkan.h>

class VKQueue : public IQueue
{
public:
	~VKQueue() override final = default;

	void CreateQueue(IDevice* _device, Type _queueType) override final;

	void SubmitCommandBuffer(ICommandBuffer* _commandBuffer) override final;
	void SubmitCommandBuffer(ICommandBuffer* _commandBuffer, IFence* _fence, ISyncPoint* _waitPoint, ISyncPoint* _signalPoint) override final;

	void WaitIdle() override final;

	VKQueue* CastToVK() override final { return this; }
	DX12Queue* CastToDX12() override final { return nullptr; }
	OGLQueue* CastToOGL() override final { return nullptr; }

	VkQueue GetRaw() const { return m_Queue; }

private:

	struct Families
	{
		int8_t graphicsFamily = -1;
		int8_t graphicsQueueCount = -1;

		int8_t computeFamily = -1;
		int8_t computeQueueCount = -1;

		int8_t transferFamily = -1;
		int8_t transferQueueCount = -1;
	};

	static Families GetQueueFamilies(VkPhysicalDevice _physicalDevice);

	static Families m_Families;

	VkQueue m_Queue = VK_NULL_HANDLE;
};

#endif //HONOU_RHI_VKPIPELINE_H