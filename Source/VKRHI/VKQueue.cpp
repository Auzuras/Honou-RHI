#include <VKRHI/VKQueue.h>

#include <VKRHI/VKDevice.h>
#include <VKRHI/VKCommandBuffer.h>
#include <VKRHI/VKFence.h>

#include <vector>

void VKQueue::CreateQueue(IDevice* _device, Type _queueType)
{
	const VkDevice lDevice = _device->CastToVK()->GetLogicalDevice();
	const VkPhysicalDevice pDevice = _device->CastToVK()->GetPhysicalDevice();

	_device->SetGraphicsQueue(this);

	Families families = VKQueue::GetQueueFamilies(pDevice);

	vkGetDeviceQueue(lDevice, 0, 0, &m_Queue);
}

void VKQueue::SubmitCommandBuffer(ICommandBuffer* _commandBuffer, const SyncPoint& _waitPoint, const SyncPoint& _signalPoint)
{
	const VkCommandBuffer cmdBuffer = _commandBuffer->CastToVK()->GetRaw();

	VkPipelineStageFlags waitStages = VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT;

	const VkSemaphore waitPoint = _waitPoint.fence ? _waitPoint.fence->CastToVK()->GetRaw() : VK_NULL_HANDLE;
	const VkSemaphore signalPoint = _signalPoint.fence ? _signalPoint.fence->CastToVK()->GetRaw() : VK_NULL_HANDLE;

	const VkTimelineSemaphoreSubmitInfo timelineSemaphoreInfo{
		.sType = VK_STRUCTURE_TYPE_TIMELINE_SEMAPHORE_SUBMIT_INFO,

		.waitSemaphoreValueCount = _waitPoint.fence ? 1ul : 0ul,
		.pWaitSemaphoreValues = &_waitPoint.fenceValue,

		.signalSemaphoreValueCount = _signalPoint.fence ? 1ul : 0ul,
		.pSignalSemaphoreValues = &_signalPoint.fenceValue
	};

	const VkSubmitInfo submitInfo{
		.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
		.pNext = &timelineSemaphoreInfo,

		.waitSemaphoreCount = _waitPoint.fence ? 1ul : 0ul,
		.pWaitSemaphores = &waitPoint,
		.pWaitDstStageMask = &waitStages,

		.commandBufferCount = 1,
		.pCommandBuffers = &cmdBuffer,

		.signalSemaphoreCount = _signalPoint.fence ? 1ul : 0ul,
		.pSignalSemaphores = &signalPoint
	};

	vkQueueSubmit(m_Queue, 1, &submitInfo, VK_NULL_HANDLE);
}

void VKQueue::WaitIdle()
{
	vkQueueWaitIdle(m_Queue);
}

VKQueue::Families VKQueue::GetQueueFamilies(VkPhysicalDevice _physicalDevice)
{
	Families families{};

	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(_physicalDevice, &queueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(_physicalDevice, &queueFamilyCount, queueFamilies.data());

	const size_t familySize = queueFamilies.size();
	for (size_t i = 0; i < familySize; ++i)
	{
		const VkQueueFamilyProperties& queueFamily = queueFamilies[i];

		if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
		{
			families.graphicsFamily = i;
			families.graphicsQueueCount = queueFamily.queueCount;
		}
		else if (queueFamily.queueFlags & VK_QUEUE_COMPUTE_BIT)
		{
			families.computeFamily = i;
			families.computeQueueCount = queueFamily.queueCount;
		}
		else if (queueFamily.queueFlags & VK_QUEUE_TRANSFER_BIT)
		{ 
			families.transferFamily = i;
			families.transferQueueCount = queueFamily.queueCount;
		}
	}

	return families;
}