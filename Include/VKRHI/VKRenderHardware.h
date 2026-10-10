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
	
	IPipeline* CreatePipeline(IDevice* const _device) override final;
	ICommandAllocator* CreateCommandAllocator(IDevice* const _device) override final;
};

#endif //HONOU_RHI_VK_RENDER_HARDWARE_H
