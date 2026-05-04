#ifndef HONOU_RENDER_IPIPELINE_H
#define HONOU_RENDER_IPIPELINE_H

class VKPipeline;
class DX12Pipeline;
class OGLPipeline;

class IDevice;

class IPipeline
{
public:
	virtual ~IPipeline() = default;

	virtual void CreatePipeline(IDevice* _device) = 0;
	virtual void DestroyPipeline() = 0;

	virtual VKPipeline* CastToVK() { return nullptr; }
	virtual DX12Pipeline* CastToDX12() { return nullptr; }
	virtual OGLPipeline* CastToOGL() { return nullptr; }
};

#endif //HONOU_RENDER_IPIPELINE_H