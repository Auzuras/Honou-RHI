#ifndef HONOU_RHI_DX12PIPELINE_H
#define HONOU_RHI_DX12PIPELINE_H

#include <RHI/IPipeline.h>

#include <wrl.h>
#include <d3d12.h>

class DX12Pipeline : public IPipeline
{
public:
	~DX12Pipeline() override final = default;

	void CreatePipeline(IDevice* _device, const GraphicsDescription& _pipelineDescription) override final;
	void CreatePipeline(IDevice* _device, const ComputeDescription& _pipelineDescription) override final;
	void DestroyPipeline(IDevice* _device) override final;

	ID3D12PipelineState* const GetRaw() { return m_Pipeline.Get(); }
	ID3D12RootSignature* const GetLayout() { return m_PipelineLayout.Get(); }

	VKPipeline* CastToVK() override final { return nullptr; }
	DX12Pipeline* CastToDX12() override final { return this; }
	OGLPipeline* CastToOGL() override final { return nullptr; }

private:

	Microsoft::WRL::ComPtr<ID3D12PipelineState> m_Pipeline = nullptr;
	Microsoft::WRL::ComPtr<ID3D12RootSignature> m_PipelineLayout = nullptr;
};

#endif //HONOU_RHI_DX12PIPELINE_H