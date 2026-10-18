#include <DX12RHI/DX12Buffer.h>

#include <DX12RHI/DX12Device.h>

void DX12Buffer::CreateBuffer(IDevice* _device, const Description& _description)
{
	ID3D12Device* const device = _device->CastToDX12()->GetRaw();

	const D3D12_HEAP_PROPERTIES heapProperties{
		.Type					=	D3D12_HEAP_TYPE_UPLOAD,
		.CPUPageProperty		=	D3D12_CPU_PAGE_PROPERTY_UNKNOWN,
		.MemoryPoolPreference	=	D3D12_MEMORY_POOL_UNKNOWN,
		.CreationNodeMask		=	1,
		.VisibleNodeMask		=	1
	};

	const D3D12_RESOURCE_DESC resourceDesc{
		.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER,
		.Alignment = 0,
		.Width = _description.size,
		.Height = 1,
		.DepthOrArraySize = 1,
		.MipLevels = 1,
		.Format = DXGI_FORMAT_UNKNOWN,
		.SampleDesc{
			.Count = 1,
			.Quality = 0
		},
		.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR,
		.Flags = D3D12_RESOURCE_FLAG_NONE
	};

	HRESULT result = device->CreateCommittedResource(&heapProperties, D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_COMMON, nullptr, IID_PPV_ARGS(&m_Buffer));

	if (FAILED(result))
	{

	}

	const D3D12_RANGE readRange{ 0, 0 };
	void* data{};
	result = m_Buffer->Map(0, &readRange, &data);

	if (FAILED(result))
	{

	}

	memcpy(data, _description.data, _description.size);

	m_Buffer->Unmap(0, nullptr);
}

void DX12Buffer::DestroyBuffer(IDevice* _device)
{

}

void DX12Buffer::SetName(IDevice* _device, const char* _name)
{

}