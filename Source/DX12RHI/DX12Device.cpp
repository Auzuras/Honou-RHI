#include <DX12RHI/DX12Device.h>
#include <DX12RHI/DX12Instance.h>

#include <dxgi1_6.h>

void DX12Device::CreateDevice(IInstance* _instance)
{
	Microsoft::WRL::ComPtr<IDXGIFactory6> factory = _instance->CastToDX12()->GetRaw();

	Microsoft::WRL::ComPtr<IDXGIAdapter3> adapter;

	HRESULT result = factory->EnumAdapterByGpuPreference(0, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&adapter));
	if (FAILED(result))
	{
		// TODO: ERROR
		return;
	}

	result = D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_Device));
	if (FAILED(result))
	{
		// TODO: ERROR
		return;
	}
}

void DX12Device::DestroyDevice()
{
	m_Device = nullptr;
}

void DX12Device::WaitIdle()
{

}