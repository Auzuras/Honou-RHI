#ifndef HONOU_RHI_IRENDER_HARDWARE_H
#define HONOU_RHI_IRENDER_HARDWARE_H

class IIstance;
class IDevice;
class ISwapChain;
class IPipeline;
class ICommandAllocator;

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
	virtual IDevice* CreateDevice(const IInstance* const _instance) = 0;
	virtual ISwapChain* CreateSwapChain(const IDevice* const _device) = 0;
	virtual IPipeline* CreatePipeline(const IDevice* const _device) = 0;
	virtual ICommandAllocator* CreateCommandAllocator(const IDevice* const _device) = 0;
};

#endif //HONOU_RHI_IRENDER_HARDWARE_H
