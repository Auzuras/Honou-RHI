#include <DX12RHI/DX12Fence.h>

#include <DX12RHI/DX12Device.h>

void DX12Fence::CreateFence(IDevice* _device)
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
	m_Event = nullptr;
}

void DX12Fence::SignalGPU(IDevice* _device, uint64_t _value)
{
	m_Fence->Signal(_value);
}

void DX12Fence::WaitCPU(IDevice* _device, uint64_t _value)
{
	if (m_Fence->GetCompletedValue() >= _value) 
		return;

	m_Fence->SetEventOnCompletion(_value, m_Event);
	WaitForSingleObject(m_Event, INFINITE);
}

uint64_t DX12Fence::GetCompletedValue(IDevice* _device)
{
	return m_Fence->GetCompletedValue();
}