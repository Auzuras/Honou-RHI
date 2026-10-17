#include <DX12RHI/DX12Fence.h>

#include <DX12RHI/DX12Device.h>

void DX12Fence::CreateFence(IDevice* _device, bool _signaled = true)
{
	Microsoft::WRL::ComPtr<ID3D12Device> device = _device->CastToDX12()->GetRaw();

	const HRESULT result = device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_Fence));

	if (FAILED(result))
	{

	}
}

void DX12Fence::DestroyFence(IDevice* _device)
{
	m_Fence = nullptr;
}

void DX12Fence::Wait(IDevice* _device)
{
	
}

void DX12Fence::Reset(IDevice* _device)
{
}