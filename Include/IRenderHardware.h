#ifndef HONOU_RHI_IRENDER_HARDWARE_H
#define HONOU_RHI_IRENDER_HARDWARE_H

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
};

#endif //HONOU_RHI_IRENDER_HARDWARE_H
