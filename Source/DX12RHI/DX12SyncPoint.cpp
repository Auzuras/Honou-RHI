#include <DX12RHI/DX12SyncPoint.h>

#include <DX12RHI/DX12Device.h>

void DX12SyncPoint::CreateSyncPoint(IDevice* _device)
{
	ID3D12Device* const device = _device->CastToDX12()->GetRaw();

	device->CreateFence(m_Value, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_Fence));
}

void DX12SyncPoint::DestroySyncPoint(IDevice* _device)
{
	m_Fence = nullptr;
}
