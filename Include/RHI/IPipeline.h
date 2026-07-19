#ifndef HONOU_RHI_IPIPELINE_H
#define HONOU_RHI_IPIPELINE_H

class VKPipeline;
class DX12Pipeline;
class OGLPipeline;

class IDevice;
class IShaderModule;

class IPipeline
{
public:

	struct ShaderDescription
	{
		IShaderModule* shader = nullptr;
		const char* entryPoint = "main";
	};

	struct GraphicsPipelineDescription
	{
		ShaderDescription vertexShader;
		ShaderDescription fragmentShader;
	};

	struct ComputePipelineDescription
	{
		ShaderDescription computeShader;
	};

	enum class PipelineType
	{
		HOUNOU_PIPELINE_TYPE_GRAPHICS	=	0,
		HOUNOU_PIPELINE_TYPE_COMPUTE	=	1,
	};

	virtual ~IPipeline() = default;

	virtual void CreatePipeline(IDevice* _device, const GraphicsPipelineDescription& _pipelineDescription) = 0;
	virtual void CreatePipeline(IDevice* _device, const ComputePipelineDescription& _pipelineDescription) = 0;
	virtual void DestroyPipeline(IDevice* _device) = 0;

	PipelineType GetPipelineType() { return p_PipelineType; }

	virtual VKPipeline* CastToVK() { return nullptr; }
	virtual DX12Pipeline* CastToDX12() { return nullptr; }
	virtual OGLPipeline* CastToOGL() { return nullptr; }

protected:

	PipelineType p_PipelineType = PipelineType::HOUNOU_PIPELINE_TYPE_GRAPHICS;
};

#endif //HONOU_RHI_IPIPELINE_H