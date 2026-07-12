#include <VKRHI/VKQueue.h>

#include <VKRHI/VKDevice.h>
#include <VKRHI/VKCommandBuffer.h>
#include <VKRHI/VKFence.h>
#include <VKRHI/VKSyncPoint.h>

#include <vector>

void VKQueue::SubmitCommandBuffer(ICommandBuffer* _commandBuffer, IFence* _fence, ISyncPoint* _waitPoint, ISyncPoint* _signalPoint)
{
	const VkCommandBuffer cmdBuffer = _commandBuffer->CastToVK()->GetRaw();

	const VkFence fence = _fence->CastToVK()->GetRaw();
	const VkSemaphore waitPoint = _waitPoint->CastToVK()->GetRaw();
	const VkSemaphore signalPoint = _signalPoint->CastToVK()->GetRaw();

	VkPipelineStageFlags waitStages = VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT;

	const VkSubmitInfo submitInfo{
		.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,

		.waitSemaphoreCount = 1,
		.pWaitSemaphores = &waitPoint,
		.pWaitDstStageMask = &waitStages,

		.commandBufferCount = 1,
		.pCommandBuffers = &cmdBuffer,

		.signalSemaphoreCount = 1,
		.pSignalSemaphores = &signalPoint
	};

	vkQueueSubmit(m_Queue, 1, &submitInfo, fence);
}

void VKQueue::CreateQueue(IDevice* _device)
{
	const VkDevice lDevice = _device->CastToVK()->GetLogicalDevice();
	const VkPhysicalDevice pDevice = _device->CastToVK()->GetPhysicalDevice();

	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(pDevice, &queueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(pDevice, &queueFamilyCount, queueFamilies.data());

	int value = 0;

	const size_t familySize = queueFamilies.size();
	for (size_t i = 0; i < familySize; ++i)
	{
		const VkQueueFamilyProperties& queueFamily = queueFamilies[i];
		if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
			value = i;
	}

	vkGetDeviceQueue(lDevice, value, 0, &m_Queue);
}