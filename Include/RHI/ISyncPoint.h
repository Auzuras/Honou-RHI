#ifndef HONOU_RHI_ISYNCPOINT_H
#define HONOU_RHI_ISYNCPOINT_H

class VKSyncPoint;
class DX12SyncPoint;
class OGLSyncPoint;

class IDevice;

class ISyncPoint
{
public:
    virtual ~ISyncPoint() = default;

    virtual void CreateSyncPoint(IDevice* _device) = 0;
    virtual void DestroySyncPoint(IDevice* _device) = 0;

    virtual VKSyncPoint* CastToVK() { return nullptr; }
    virtual DX12SyncPoint* CastToDX12() { return nullptr; }
    virtual OGLSyncPoint* CastToOGL() { return nullptr; }
};

#endif //HONOU_RHI_ISYNCPOINT_H