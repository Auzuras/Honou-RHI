#ifndef HONOU_RHI_VK_RENDER_HARDWARE_H
#define HONOU_RHI_VK_RENDER_HARDWARE_H

#include <RHI/IRenderHardware.h>

class VKRenderHardware : public IRenderHardware
{
public:
	~VKRenderHardware() override final = default;

	void Initialize(RenderHardware _renderHardware = RenderHardware::HOUNOU_RENDER_HARDWARE_VULKAN) override final;
	void Terminate() override final;

	IInstance* CreateInstance() override final;
	void DestroyInstance(IInstance* _instance) override final;

	IDevice* CreateDevice(IInstance* const _instance) override final;
	void DestroyDevice(IDevice* _device) override final;

	ISwapChain* CreateSwapChain(IInstance* const _instance, IDevice* const _device, const ISwapChain::Description& _description) override final;
	void DestroySwapChain(IInstance* const _instance, IDevice* const _device, ISwapChain* _swapChain) override final;
	
	IPipeline* CreatePipeline(IDevice* const _device, const IPipeline::GraphicsDescription& _description) override final;
	IPipeline* CreatePipeline(IDevice* const _device, const IPipeline::ComputeDescription& _description) override final;
	void DestroyPipeline(IDevice* const _device, IPipeline* _pipeline) override final;

	ICommandAllocator* CreateCommandAllocator(IDevice* const _device) override final;
	void DestroyCommandAllocator(IDevice* const _device, ICommandAllocator* _commandAllocator) override final;

	IDescriptorAllocator* CreateDescriptorAllocator(IDevice* const _device) override final;
	void DestroyDescriptorAllocator(IDevice* const _device, IDescriptorAllocator* _descriptorAllocator) override final;

	IImage* CreateImage(IDevice* const _device, const IImage::Description& _description) override final;
	void DestroyImage(IDevice* const _device, IImage* _image) override final;

	IBuffer* CreateBuffer(IDevice* const _device, const IBuffer::Description& _description) override final;
	void DestroyBuffer(IDevice* const _device, IBuffer* _buffer) override final;

	ISampler* CreateSampler(IDevice* const _device, const ISampler::Description& _description) override final;
	void DestroySampler(IDevice* const _device, ISampler* _sampler) override final;
};

#endif //HONOU_RHI_VK_RENDER_HARDWARE_H
