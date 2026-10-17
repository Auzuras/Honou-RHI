#ifndef HONOU_RHI_DX12SHADERMODULE_H
#define HONOU_RHI_DX12SHADERMODULE_H

#include <RHI/IShaderModule.h>

#include <wrl.h>
#include <d3d12.h>

class DX12ShaderModule : public IShaderModule
{
public:
	~DX12ShaderModule() override final = default;

	void CreateShaderModule(IDevice* _device, const std::vector<char>& _data) override final;
	void DestroyShaderModule(IDevice* _device) override final;

	ID3DBlob* const GetRaw() const { return m_ShaderModule.Get(); }

	VKShaderModule* const CastToVK() override final { return nullptr; }
	DX12ShaderModule* CastToDX12() override final { return this; }
	OGLShaderModule* CastToOGL() override final { return nullptr; }

private:

	Microsoft::WRL::ComPtr<ID3DBlob> m_ShaderModule = nullptr;
};

#endif //HONOU_RHI_DX12SHADERMODULE_H
