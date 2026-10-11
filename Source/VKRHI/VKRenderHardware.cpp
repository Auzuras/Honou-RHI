#include <VKRHI/VKRenderHardware.h>

#include <VKRHI/VKInstance.h>
#include <VKRHI/VKDevice.h>

#include <VKRHI/VKSwapChain.h>
#include <VKRHI/VKPipeline.h>

#include <VKRHI/VKCommandAllocator.h>
#include <VKRHI/VKDescriptorAllocator.h>

#include <VKRHI/VKImage.h>
#include <VKRHI/VKBuffer.h>
#include <VKRHI/VKSampler.h>

void VKRenderHardware::Initialize(RenderHardware _renderHardware)
{

}

void VKRenderHardware::Terminate()
{

}

IInstance* VKRenderHardware::CreateInstance()
{
	IInstance* instance = new VKInstance();
	instance->CreateInstance();
	return instance;
}

void VKRenderHardware::DestroyInstance(IInstance* _instance)
{
	_instance->DestroyInstance();
	delete _instance;
}

IDevice* VKRenderHardware::CreateDevice(IInstance* const _instance)
{
	IDevice* device = new VKDevice();
	device->CreateDevice(_instance);
	return device;
}

void VKRenderHardware::DestroyDevice(IDevice* _device)
{
	_device->DestroyDevice();
	delete _device;
}

ISwapChain* VKRenderHardware::CreateSwapChain(IInstance* const _instance, IDevice* const _device, const ISwapChain::Description& _description)
{
	ISwapChain* swapChain = new VKSwapChain();
	swapChain->CreateSwapChain(_instance, _device, _description);
	return swapChain;
}

void VKRenderHardware::DestroySwapChain(IInstance* const _instance, IDevice* const _device, ISwapChain* _swapChain)
{
	_swapChain->DestroySwapChain(_instance, _device);
	delete _swapChain;
}

IPipeline* VKRenderHardware::CreatePipeline(IDevice* const _device, const IPipeline::GraphicsDescription& _description)
{
	IPipeline* pipeline = new VKPipeline();
	pipeline->CreatePipeline(_device, _description);
	return pipeline;
}

IPipeline* VKRenderHardware::CreatePipeline(IDevice* const _device, const IPipeline::ComputeDescription& _description)
{
	IPipeline* pipeline = new VKPipeline();
	pipeline->CreatePipeline(_device, _description);
	return pipeline;
}

void VKRenderHardware::DestroyPipeline(IDevice* const _device, IPipeline* _pipeline)
{
	_pipeline->DestroyPipeline(_device);
	delete _pipeline;
}

ICommandAllocator* VKRenderHardware::CreateCommandAllocator(IDevice* const _device)
{
	ICommandAllocator* commandAllocator = new VKCommandAllocator();
	commandAllocator->CreateCommandAllocator(_device);
	return commandAllocator;
}

void VKRenderHardware::DestroyCommandAllocator(IDevice* const _device, ICommandAllocator* _commandAllocator)
{
	_commandAllocator->DestroyCommandAllocator(_device);
	delete _commandAllocator;
}

IDescriptorAllocator* VKRenderHardware::CreateDescriptorAllocator(IDevice* const _device)
{
	IDescriptorAllocator* descriptorAllocator = new VKDescriptorAllocator();
	descriptorAllocator->CreateDescriptorAllocator(_device);
	return descriptorAllocator;
}

void VKRenderHardware::DestroyDescriptorAllocator(IDevice* const _device, IDescriptorAllocator* _descriptorAllocator)
{
	_descriptorAllocator->DestroyDescriptorAllocator(_device);
	delete _descriptorAllocator;
}

IImage* VKRenderHardware::CreateImage(IDevice* const _device, const IImage::Description& _description)
{
	IImage* image = new VKImage();
	image->CreateImage(_device, _description);
	return image;
}

void VKRenderHardware::DestroyImage(IDevice* const _device, IImage* _image)
{
	_image->DestroyImage(_device);
	delete _image;
}

IBuffer* VKRenderHardware::CreateBuffer(IDevice* const _device, const IBuffer::Description& _description)
{
	IBuffer* const buffer = new VKBuffer();
	buffer->CreateBuffer(_device, _description);
	return buffer;
}

void VKRenderHardware::DestroyBuffer(IDevice* const _device, IBuffer* _buffer)
{
	_buffer->DestroyBuffer(_device);
	delete _buffer;
}

ISampler* VKRenderHardware::CreateSampler(IDevice* const _device, const ISampler::Description& _description)
{
	ISampler* sampler = new VKSampler();
	sampler->CreateSampler(_device, _description);
	return sampler;
}

void VKRenderHardware::DestroySampler(IDevice* const _device, ISampler* _sampler)
{
	_sampler->DestroySampler(_device);
	delete _sampler;
}