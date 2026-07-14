#include <DX12RHI/DX12Instance.h>

#include <d3d12sdklayers.h>
#include <dxgidebug.h>

void DX12Instance::CreateInstance()
{
	UINT dxgiFactoryFlags = 0;

	//Microsoft::WRL::ComPtr<ID3D12Debug1> debugController = nullptr;
	//const HRESULT debuggerResult = D3D12GetDebugInterface(IID_PPV_ARGS(&debugController));
	//
	//if (SUCCEEDED(debuggerResult))
	//{
	//	debugController->EnableDebugLayer();
	//	debugController->SetEnableGPUBasedValidation(true);
	//}
	//else
	//{
	//	// TODO: Error
	//}

	//Microsoft::WRL::ComPtr<IDXGIInfoQueue> dxgiInfoQueue = nullptr;
	//const HRESULT debugInterfaceResult = DXGIGetDebugInterface1(0, IID_PPV_ARGS(&dxgiInfoQueue));

	//if (SUCCEEDED(debugInterfaceResult))
	//{
	//	dxgiInfoQueue->SetBreakOnSeverity(DXGI_DEBUG_ALL, DXGI_INFO_QUEUE_MESSAGE_SEVERITY_CORRUPTION, true);
	//	dxgiInfoQueue->SetBreakOnSeverity(DXGI_DEBUG_ALL, DXGI_INFO_QUEUE_MESSAGE_SEVERITY_ERROR, true);
	//	dxgiInfoQueue->SetBreakOnSeverity(DXGI_DEBUG_ALL, DXGI_INFO_QUEUE_MESSAGE_SEVERITY_WARNING, true);
	//}
	//else
	//{
	//	// TODO: Error
	//}

	//dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;

	const HRESULT factoryResult = CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&m_Factory));
	
	if (FAILED(factoryResult))
	{
		// TODO: Error
		return;
	}
}

void DX12Instance::DestroyInstance()
{
	m_Factory = nullptr;
}