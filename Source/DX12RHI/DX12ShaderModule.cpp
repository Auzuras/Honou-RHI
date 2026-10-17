#include <DX12RHI/DX12ShaderModule.h>

#include <DX12RHI/DX12Device.h>
#include <d3dcompiler.h>

void DX12ShaderModule::CreateShaderModule(IDevice* _device, const std::vector<char>& _data)
{
	ID3D12Device* const device = _device->CastToDX12()->GetRaw();

	const HRESULT result = D3DCreateBlob(_data.size(), &m_ShaderModule);
	
	if (FAILED(result))
	{
		return;
	}

	memcpy(m_ShaderModule->GetBufferPointer(), _data.data(), _data.size());
}

void DX12ShaderModule::DestroyShaderModule(IDevice* _device)
{
	m_ShaderModule = nullptr;
}