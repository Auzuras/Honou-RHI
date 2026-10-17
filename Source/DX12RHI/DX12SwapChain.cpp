#include <DX12RHI/DX12SwapChain.h>

#include <DX12RHI/DX12Instance.h>
#include <DX12RHI/DX12Device.h>

void DX12SwapChain::CreateSwapChain(IInstance* _instance, IDevice* _device, const Description& _description)
{
	// SwapChain creation
	Microsoft::WRL::ComPtr<IDXGIFactory6> instance = _instance->CastToDX12()->GetRaw();
	Microsoft::WRL::ComPtr<ID3D12Device> device = _device->CastToDX12()->GetRaw();

	const DXGI_SWAP_CHAIN_DESC1 scDesc{
		.Width			=	_description.width,
		.Height			=	_description.height,
		.Format			=	DXGI_FORMAT_R8G8B8A8_UNORM,
		.Stereo			=	false,
		.SampleDesc = {
			.Count		=	1,
			.Quality	=	0
		},
		.BufferUsage	=	DXGI_USAGE_RENDER_TARGET_OUTPUT,
		.BufferCount	=	ISwapChain::MAX_FRAMES_IN_FLIGHT,
		.Scaling		=	DXGI_SCALING_STRETCH,
		.SwapEffect		=	DXGI_SWAP_EFFECT_FLIP_DISCARD,
		.AlphaMode		=	DXGI_ALPHA_MODE_UNSPECIFIED,
		.Flags			=	0
	};

	Microsoft::WRL::ComPtr<IDXGISwapChain1> sc1;
	HRESULT result = instance->CreateSwapChainForHwnd(device.Get(), _description.hwnd, &scDesc, nullptr, nullptr, &sc1);

	if (FAILED(result))
	{

	}
	else
	{

	}

	result = sc1.As(&m_SwapChain);

	if (FAILED(result))
	{

	}
	else
	{

	}

	// Retrieve backbuffers

	for (size_t i = 0; i < ISwapChain::MAX_FRAMES_IN_FLIGHT; ++i)
	{
		Microsoft::WRL::ComPtr<ID3D12Resource> image;
		result = m_SwapChain->GetBuffer(i, IID_PPV_ARGS(&image));
		
		if (FAILED(result))
		{

		}
		else
		{

		}
	}

}