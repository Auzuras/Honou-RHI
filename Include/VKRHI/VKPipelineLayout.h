#ifndef HONOU_RHI_VKPIPELINE_LAYOUT_H
#define HONOU_RHI_VKPIPELINE_LAYOUT_H

#include <RHI/IPipelineLayout.h>

#include <vulkan/vulkan.h>

class VKPipelineLayout : public IPipelineLayout
{
public:
	~VKPipelineLayout() override final = default;

	void CreatePipelineLayout(IDevice* _device) override final;
	void DestroyPipelineLayout(IDevice* _device) override final;

	VkPipelineLayout GetLayout() const { return m_PipelineLayout; }
	VkDescriptorSetLayout GetDescriptorLayout() const { return m_DescriptorSetLayout; }

	VKPipelineLayout* CastToVK() override final { return this; }
	DX12PipelineLayout* CastToDX12() override final { return nullptr; }
	OGLPipelineLayout* CastToOGL() override final { return nullptr; }

private:

	VkDescriptorSetLayout m_DescriptorSetLayout = VK_NULL_HANDLE;
	VkPipelineLayout m_PipelineLayout = VK_NULL_HANDLE;
};

#endif //HONOU_RHI_VKPIPELINE_LAYOUT_H