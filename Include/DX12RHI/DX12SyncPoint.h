#ifndef HONOU_RHI_DX12SYNCPOINT_H
#define HONOU_RHI_DX12SYNCPOINT_H

#include <RHI/ISyncPoint.h>

#include <wrl.h>
#include <d3d12.h>

class DX12SyncPoint : public ISyncPoint
{
public:
    ~DX12SyncPoint() override final = default;

    void CreateSyncPoint(IDevice* _device) override final;
    void DestroySyncPoint(IDevice* _device) override final;

    VKSyncPoint* CastToVK() override final { return nullptr; }
    DX12SyncPoint* CastToDX12() override final { return this; }
    OGLSyncPoint* CastToOGL() override final { return nullptr; }

private:

    Microsoft::WRL::ComPtr<ID3D12Fence> m_Fence = nullptr;
    UINT64 m_Value = 0;
};

#endif //HONOU_RHI_DX12SYNCPOINT_H