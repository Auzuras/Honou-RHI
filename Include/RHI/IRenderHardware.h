#ifndef HONOU_RHI_IRENDER_HARDWARE_H
#define HONOU_RHI_IRENDER_HARDWARE_H

#include <RHI/IInstance.h>
#include <RHI/IDevice.h>
#include <RHI/ISwapChain.h>
#include <RHI/IPipeline.h>
#include <RHI/ICommandAllocator.h>

#include <cstdint>

class IRenderHardware
{
public:

	enum class RenderHardware : uint8_t
	{
		HOUNOU_RENDER_HARDWARE_VULKAN		=	0,
		HOUNOU_RENDER_HARDWARE_DIRECTX12	=	1,
		HOUNOU_RENDER_HARDWARE_OPENGL		=	2
	};

public:
	virtual ~IRenderHardware() = default;

	virtual void Initialize(RenderHardware _renderHardware = RenderHardware::HOUNOU_RENDER_HARDWARE_VULKAN) = 0;
	virtual void Terminate() = 0;

	virtual IInstance* CreateInstance() = 0;
	virtual void DestroyInstance(IInstance* _instance) = 0;

	virtual IDevice* CreateDevice(IInstance* const _instance) = 0;
	virtual void DestroyDevice(IDevice* _device) = 0;

	virtual ISwapChain* CreateSwapChain(IInstance* const _instance, IDevice* const _device, const ISwapChain::Description& _description) = 0;
	virtual void DestroySwapChain(IInstance* const _instance, IDevice* const _device, ISwapChain* _swapChain) = 0;

	virtual IPipeline* CreatePipeline(IDevice* const _device) = 0;
	virtual ICommandAllocator* CreateCommandAllocator(IDevice* const _device) = 0;
};

#endif //HONOU_RHI_IRENDER_HARDWARE_H
