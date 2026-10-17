#include <DX12RHI/DX12Image.h>

#include <DX12RHI/DX12Device.h>

void DX12Image::CreateImage(IDevice* _device, const Description& _description)
{
	Microsoft::WRL::ComPtr<ID3D12Device> device = _device->CastToDX12()->GetRaw();

	const D3D12_HEAP_PROPERTIES heap{
		.Type = D3D12_HEAP_TYPE_DEFAULT
	};

	const D3D12_RESOURCE_DESC resourceDesc{
		.Dimension			=	D3D12_RESOURCE_DIMENSION_TEXTURE2D,
		.Alignment			=	0,
		.Width				=	_description.width,
		.Height				=	_description.height,
		.DepthOrArraySize	=	1,
		.MipLevels			=	1,
		.Format				=	MapToDX12Format(_description.format),
		.SampleDesc = {
			.Count			=	1,
			.Quality		=	0
		},
		.Layout				=	D3D12_TEXTURE_LAYOUT_UNKNOWN,
		.Flags				=	D3D12_RESOURCE_FLAG_NONE
	};

	HRESULT result = device->CreateCommittedResource(&heap, D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_COPY_DEST, nullptr, IID_PPV_ARGS(&m_Image));

	if (FAILED(result))
	{

	}
	else
	{

	}
}