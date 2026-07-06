#include <gtest/gtest.h>

#include <VKRHI/VKInstance.h>
#include <VKRHI/VKDevice.h>
#include <VKRHI/VKSurface.h>
#include <VKRHI/VKSwapChain.h>
#include <VKRHI/VKPipeline.h>
#include <VKRHI/VKCommandAllocator.h>
#include <VKRHI/VKCommandBuffer.h>
#include <VKRHI/VKImage.h>
#include <VKRHI/VKBuffer.h>
#include <VKRHI/VKCommandAllocator.h>
#include <VKRHI/VKShaderModule.h>

#include <Window/Window.h>

class VKRHITest : public testing::Test {};

TEST(VKRHITest, InstanceTest)
{
	VKInstance instance;
	instance.CreateInstance();
	instance.DestroyInstance();
}

TEST(VKRHITest, DeviceTest)
{
	VKInstance instance;
	instance.CreateInstance();

	VKDevice device;
	device.CreateDevice(&instance);
	device.DestroyDevice();

	instance.DestroyInstance();
}

TEST(VKRHITest, SurfaceTest)
{
	VKInstance instance;
	instance.CreateInstance();
	
	Window window(1920, 1080);

	VKSurface surface;
	ISurface::SurfaceInformation info{
		.instance = GetModuleHandle(NULL),
		.hwnd = window.GetHandle()
	};

	surface.CreateSurface(&instance, info);
	surface.DestroySurface(&instance);

	instance.DestroyInstance();

	window.KooriDestroyWindow();
}

TEST(VKRHITest, SwapChainTest)
{
	VKInstance instance;
	instance.CreateInstance();

	Window window(1920, 1080);

	VKSurface surface;
	ISurface::SurfaceInformation info{
		.instance = GetModuleHandle(NULL),
		.hwnd = window.GetHandle()
	};

	surface.CreateSurface(&instance, info);

	VKDevice device;
	device.CreateDevice(&instance);

	VKSwapChain swapChain;
	swapChain.CreateSwapChain(&device, &surface);
	swapChain.DestroySwapChain(&device);

	device.DestroyDevice();
	surface.DestroySurface(&instance);
	instance.DestroyInstance();

	window.KooriDestroyWindow();
}

TEST(VKRHITest, PipelineTest)
{
	VKInstance instance;
	instance.CreateInstance();

	VKDevice device;
	device.CreateDevice(&instance);

	VKPipeline pipeline;
	pipeline.CreatePipeline(&device);
	pipeline.DestroyPipeline(&device);

	device.DestroyDevice();
	instance.DestroyInstance();
}

TEST(VKRHITest, CommandAllocatorTest)
{
	VKInstance instance;
	instance.CreateInstance();

	VKDevice device;
	device.CreateDevice(&instance);

	VKCommandAllocator cmdAlloc;
	cmdAlloc.CreateCommandAllocator(&device);

	ICommandBuffer* cmdBuffer = cmdAlloc.AllocateCommandBuffer(&device);
	cmdAlloc.FreeCommandBuffer(&device, cmdBuffer);

	cmdAlloc.DestroyCommandAllocator(&device);

	device.DestroyDevice();
	instance.DestroyInstance();
}

TEST(VKRHITest, RenderLoop)
{
	VKInstance instance;
	instance.CreateInstance();

	Window window(1920, 1080);

	VKSurface surface;
	ISurface::SurfaceInformation info{
		.instance = GetModuleHandle(NULL),
		.hwnd = window.GetHandle()
	};

	surface.CreateSurface(&instance, info);

	VKDevice device;
	device.CreateDevice(&instance);

	VKSwapChain swapChain;
	swapChain.CreateSwapChain(&device, &surface);

	VKPipeline pipeline;
	pipeline.CreatePipeline(&device);

	while (!window.WindowShouldClose())
	{
		window.UpdateWindow();
	}

	pipeline.DestroyPipeline(&device);

	swapChain.DestroySwapChain(&device);

	device.DestroyDevice();
	surface.DestroySurface(&instance);
	instance.DestroyInstance();

	window.KooriDestroyWindow();
}