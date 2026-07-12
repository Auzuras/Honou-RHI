#include <VKRHI/VKSyncPoint.h>

#include <VKRHI/VKDevice.h>

void VKSyncPoint::CreateSyncPoint(IDevice* _device)
{
    const VkDevice lDevice = _device->CastToVK()->GetLogicalDevice();

    const VkSemaphoreCreateInfo semaphoreCI{
    .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO
    };

    vkCreateSemaphore(lDevice, &semaphoreCI, nullptr, &m_Semaphore);
}

void VKSyncPoint::DestroySyncPoint(IDevice* _device)
{
    const VkDevice lDevice = _device->CastToVK()->GetLogicalDevice();

    vkDestroySemaphore(lDevice, m_Semaphore, nullptr);
}
