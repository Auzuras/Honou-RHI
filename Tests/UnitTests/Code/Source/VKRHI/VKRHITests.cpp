#include <gtest/gtest.h>

#include <VKRHI/VKInstance.h>
#include <VKRHI/VKDevice.h>
#include <VKRHI/VKSwapChain.h>
#include <VKRHI/VKImage.h>
#include <VKRHI/VKBuffer.h>
#include <VKRHI/VKCommandAllocator.h>

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

TEST(VKRHITest, SwapChainTest)
{
	VKInstance instance;
	instance.CreateInstance();

	VKDevice device;
	device.CreateDevice(&instance);

	VKSwapChain swapChain;
	swapChain.CreateSwapChain(&device);
	swapChain.DestroySwapChain(&device);

	device.DestroyDevice();
	instance.DestroyInstance();
}
