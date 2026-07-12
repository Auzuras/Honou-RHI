#include <VKRHI/VKFence.h>

#include <VKRHI/VKDevice.h>

void VKFence::CreateFence(IDevice* _device, bool _signaled)
{
	const VkDevice lDevice = _device->CastToVK()->GetLogicalDevice();

	p_IsSignaled = _signaled;

	const VkFenceCreateInfo fenceCI{
		.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
		.flags = _signaled ? VK_FENCE_CREATE_SIGNALED_BIT : static_cast<VkFenceCreateFlags>(0)
	};

	vkCreateFence(lDevice, &fenceCI, nullptr, &m_Fence);
}

void VKFence::Wait(IDevice* _device)
{
	const VkDevice lDevice = _device->CastToVK()->GetLogicalDevice();
	vkWaitForFences(lDevice, 1, &m_Fence, true, UINT64_MAX);
}

void VKFence::Reset(IDevice* _device)
{
	const VkDevice lDevice = _device->CastToVK()->GetLogicalDevice();
	vkResetFences(lDevice, 1, &m_Fence);
	p_IsSignaled = false;
}

void VKFence::DestroyFence(IDevice* _device)
{
	const VkDevice lDevice = _device->CastToVK()->GetLogicalDevice();
	vkDestroyFence(lDevice, m_Fence, nullptr);
	p_IsSignaled = false;
}