#ifndef HONOU_RENDER_VKPIPELINE_H
#define HONOU_RENDER_VKPIPELINE_H

#include <RHI/IPipeline.h>

#include <vulkan/vulkan.h>

class VKPipeline : public IPipeline
{
public:
	~VKPipeline() override final = default;

	void CreatePipeline(IDevice* _device) override final = 0;
	void DestroyPipeline(IDevice* _device) override final = 0;

	VKPipeline* CastToVK() override final { return this; }
	DX12Pipeline* CastToDX12() override final { return nullptr; }
	OGLPipeline* CastToOGL() override final { return nullptr; }

private:

	VkPipelineLayout m_PipelineLayout = VK_NULL_HANDLE;
	VkPipeline m_Pipeline = VK_NULL_HANDLE;
};

#endif //HONOU_RENDER_VKPIPELINE_H