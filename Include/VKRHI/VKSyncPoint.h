#ifndef HONOU_RHI_VKSYNCPOINT_H
#define HONOU_RHI_VKSYNCPOINT_H

#include <RHI/ISyncPoint.h>

#include <vulkan/vulkan.h>

class VKSyncPoint : public ISyncPoint
{
public:
    ~VKSyncPoint() override final = default;

    void CreateSyncPoint(IDevice* _device) override final;
    void DestroySyncPoint(IDevice* _device) override final;
    
    VKSyncPoint* CastToVK() override final { return this; }
    DX12SyncPoint* CastToDX12() override final { return nullptr; }
    OGLSyncPoint* CastToOGL() override final { return nullptr; }

    VkSemaphore GetRaw() const { return m_Semaphore; }

private:

    VkSemaphore m_Semaphore = VK_NULL_HANDLE;
};

#endif //HONOU_RHI_VKSYNCPOINT_H