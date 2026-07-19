#ifndef HONOU_RHI_VKPIPELINE_H
#define HONOU_RHI_VKPIPELINE_H

#include <RHI/IPipeline.h>

#include <vulkan/vulkan.h>
#include <array>

class VKPipeline : public IPipeline
{
public:
	~VKPipeline() override final = default;

	void CreatePipeline(IDevice* _device, const GraphicsPipelineDescription& _pipelineDescription) override final;
	void CreatePipeline(IDevice* _device, const ComputePipelineDescription& _pipelineDescription) override final;
	void DestroyPipeline(IDevice* _device) override final;

	VkPipeline GetRaw() const { return m_Pipeline; }
	VkPipelineLayout GetLayout() const { return m_PipelineLayout; }
	VkDescriptorSetLayout GetDescriptorLayout() const { return m_DescriptorSetLayout; }

	VKPipeline* CastToVK() override final { return this; }
	DX12Pipeline* CastToDX12() override final { return nullptr; }
	OGLPipeline* CastToOGL() override final { return nullptr; }

private:

	VkDescriptorSetLayout m_DescriptorSetLayout = VK_NULL_HANDLE;
	VkPipelineLayout m_PipelineLayout = VK_NULL_HANDLE;
	VkPipeline m_Pipeline = VK_NULL_HANDLE;
};

#endif //HONOU_RHI_VKPIPELINE_H