#include <gtest/gtest.h>

#include <VKRHI/VKInstance.h>
#include <VKRHI/VKDevice.h>
#include <VKRHI/VKSurface.h>
#include <VKRHI/VKSwapChain.h>
#include <VKRHI/VKImage.h>
#include <VKRHI/VKBuffer.h>
#include <VKRHI/VKCommandAllocator.h>
#include <VKRHI/VKShaderModule.h>

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

	VKSurface surface;
	ISurface::SurfaceInformation info{
		.instance	=	NULL,
		.hwnd		=	NULL
	};

	surface.CreateSurface(&instance, info);
	surface.DestroySurface(&instance);

	instance.DestroyInstance();
}

TEST(VKRHITest, SwapChainTest)
{
	VKInstance instance;
	instance.CreateInstance();

	VKSurface surface;
	ISurface::SurfaceInformation info{
		.instance = NULL,
		.hwnd = NULL
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
}
