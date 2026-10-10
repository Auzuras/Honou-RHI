#include <VKRHI/VKRenderHardware.h>

#include <VKRHI/VKInstance.h>
#include <VKRHI/VKDevice.h>
#include <VKRHI/VKSwapChain.h>

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

IPipeline* VKRenderHardware::CreatePipeline(IDevice* const _device)
{
	return nullptr;
}

ICommandAllocator* VKRenderHardware::CreateCommandAllocator(IDevice* const _device)
{
	return nullptr;
}