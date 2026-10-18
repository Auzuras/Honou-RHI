#include <DX12RHI/DX12Device.h>
#include <DX12RHI/DX12Instance.h>

#include <dxgi1_6.h>
#include <iostream>

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

	Microsoft::WRL::ComPtr<ID3D12InfoQueue1> infoQueue = nullptr;
	result = m_Device->QueryInterface(IID_PPV_ARGS(&infoQueue));

	if (SUCCEEDED(result))
	{
		infoQueue->RegisterMessageCallback(DX12Device::DebugCallback, D3D12_MESSAGE_CALLBACK_IGNORE_FILTERS, nullptr, &m_VLayerCallbackCookie);
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, true);
	}
}

void DX12Device::DestroyDevice()
{
	m_Device = nullptr;
}

void DX12Device::WaitIdle()
{

}

void DX12Device::DebugCallback(D3D12_MESSAGE_CATEGORY _category, D3D12_MESSAGE_SEVERITY _severity, D3D12_MESSAGE_ID _id, LPCSTR _description, void* _context)
{
	std::cout << _severity << " " << _description << "\n";
}