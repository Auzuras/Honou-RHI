#include <VKRHI/VKQueue.h>

#include <VKRHI/VKDevice.h>
#include <VKRHI/VKCommandBuffer.h>
#include <VKRHI/VKFence.h>
#include <VKRHI/VKSyncPoint.h>

#include <vector>

void VKQueue::CreateQueue(IDevice* _device, Type _queueType)
{
	const VkDevice lDevice = _device->CastToVK()->GetLogicalDevice();
	const VkPhysicalDevice pDevice = _device->CastToVK()->GetPhysicalDevice();

	_device->SetGraphicsQueue(this);

	Families families = VKQueue::GetQueueFamilies(pDevice);

	vkGetDeviceQueue(lDevice, 0, 0, &m_Queue);
}

void VKQueue::SubmitCommandBuffer(ICommandBuffer* _commandBuffer)
{
	const VkCommandBuffer cmdBuffer = _commandBuffer->CastToVK()->GetRaw();

	const VkSubmitInfo submitInfo{
		.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
		.commandBufferCount = 1,
		.pCommandBuffers = &cmdBuffer,
	};

	vkQueueSubmit(m_Queue, 1, &submitInfo, VK_NULL_HANDLE);
}

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