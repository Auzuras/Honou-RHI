#include <DX12RHI/DX12SwapChain.h>

#include <DX12RHI/DX12Instance.h>
#include <DX12RHI/DX12Device.h>
#include <DX12RHI/DX12Image.h>
#include <DX12RHI/DX12Queue.h>

#include <string>
#include <iostream>

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

	// TODO: CHANGE IT FOR A PRESENTATION QUEUE?
	Microsoft::WRL::ComPtr<IDXGISwapChain1> sc1;
	HRESULT result = instance->CreateSwapChainForHwnd(_device->GetGraphicsQueue()->CastToDX12()->GetRaw(), _description.hwnd, &scDesc, nullptr, nullptr, &sc1);

	if (FAILED(result))
	{
		std::cout << result;
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

	CreateSwapChainImages(_device, _description.width, _description.height);
}

void DX12SwapChain::RecreateSwapChain(IDevice* _device, const Description& _description)
{}

void DX12SwapChain::DestroySwapChain(IInstance* _instance, IDevice* _device)
{}

uint32_t DX12SwapChain::GetNextSwapChainImage(IDevice* _device, uint32_t _frameIndex)
{
	return 0;
}

void DX12SwapChain::Present(IDevice* _device, IQueue* _presentQueue, ISyncPoint* _syncPoint)
{
	const HRESULT result = m_SwapChain->Present(1, 0);
	
	if (FAILED(result))
	{

	}
}

void DX12SwapChain::CreateSwapChainImages(IDevice* _device, uint32_t _width, uint32_t _height)
{
	const IImage::Description desc{
		.width = _width,
		.height = _height,
		.format = IImage::Format::HONOU_FORMAT_B8G8R8A8_UNORM
	};

	for (size_t i = 0; i < ISwapChain::MAX_FRAMES_IN_FLIGHT; ++i)
	{
		Microsoft::WRL::ComPtr<ID3D12Resource> cImage;
		const HRESULT result = m_SwapChain->GetBuffer(i, IID_PPV_ARGS(&cImage));

		if (FAILED(result))
		{

		}
		else
		{
			DX12Image* image = new DX12Image();
			image->CreateSwapChainImage(_device, cImage.Get(), desc);

			std::string name = "Hounou RHI - Swap Chain Image " + std::to_string(i);
			image->SetName(_device, name.c_str());
			p_SwapChainImages.push_back(image);
		}
	}
}

void DX12SwapChain::DestroySwapChainImages(IDevice* _device)
{}