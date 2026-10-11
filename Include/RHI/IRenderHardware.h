#ifndef HONOU_RHI_IRENDER_HARDWARE_H
#define HONOU_RHI_IRENDER_HARDWARE_H

#include <RHI/IInstance.h>
#include <RHI/IDevice.h>

#include <RHI/ISwapChain.h>
#include <RHI/IPipeline.h>

#include <RHI/ICommandAllocator.h>
#include <RHI/IDescriptorAllocator.h>

#include <RHI/IImage.h>
#include <RHI/IBuffer.h>
#include <RHI/ISampler.h>

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

	virtual IPipeline* CreatePipeline(IDevice* const _device, const IPipeline::GraphicsDescription& _description) = 0;
	virtual IPipeline* CreatePipeline(IDevice* const _device, const IPipeline::ComputeDescription& _description) = 0;
	virtual void DestroyPipeline(IDevice* const _device, IPipeline* _pipeline) = 0;

	virtual ICommandAllocator* CreateCommandAllocator(IDevice* const _device) = 0;
	virtual void DestroyCommandAllocator(IDevice* const _device, ICommandAllocator* _commandAllocator) = 0;

	virtual IDescriptorAllocator* CreateDescriptorAllocator(IDevice* const _device) = 0;
	virtual void DestroyDescriptorAllocator(IDevice* const _device, IDescriptorAllocator* _descriptorAllocator) = 0;

	virtual IImage* CreateImage(IDevice* const _device, const IImage::Description& _description) = 0;
	virtual void DestroyImage(IDevice* const _device, IImage* _image) = 0;

	virtual IBuffer* CreateBuffer(IDevice* const _device, const IBuffer::Description& _description) = 0;
	virtual void DestroyBuffer(IDevice* const _device, IBuffer* _buffer) = 0;

	virtual ISampler* CreateSampler(IDevice* const _device, const ISampler::Description& _description) = 0;
	virtual void DestroySampler(IDevice* const _device, ISampler* _sampler) = 0;
};

#endif //HONOU_RHI_IRENDER_HARDWARE_H
