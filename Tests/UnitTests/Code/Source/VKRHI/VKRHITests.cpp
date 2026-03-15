#include <gtest/gtest.h>

#include <VKRHI/VKInstance.h>
#include <VKRHI/VKDevice.h>

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
	device.CreateDevice(reinterpret_cast<IInstance*>(&instance));
	device.DestroyDevice();

	instance.DestroyInstance();
}
