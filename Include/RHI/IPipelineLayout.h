#ifndef HONOU_RHI_IPIPELINE_LAYOUT_H
#define HONOU_RHI_IPIPELINE_LAYOUT_H

class VKPipelineLayout;
class DX12PipelineLayout;
class OGLPipelineLayout;

class IDevice;

class IPipelineLayout
{
public:

	virtual ~IPipelineLayout() = default;

	virtual void CreatePipelineLayout(IDevice* _device) = 0;
	virtual void DestroyPipelineLayout(IDevice* _device) = 0;

	virtual VKPipelineLayout* CastToVK() { return nullptr; }
	virtual DX12PipelineLayout* CastToDX12() { return nullptr; }
	virtual OGLPipelineLayout* CastToOGL() { return nullptr; }
};

#endif //HONOU_RHI_IPIPELINE_LAYOUT_H