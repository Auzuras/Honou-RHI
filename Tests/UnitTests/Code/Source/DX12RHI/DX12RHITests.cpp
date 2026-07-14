#include <gtest/gtest.h>

#include <Window/Window.h>

#include <DX12RHI/DX12Instance.h>

class DX12RHITest : public testing::Test {};

TEST(DX12RHITest, InstanceTest)
{
	DX12Instance instance;
	instance.CreateInstance();
	instance.DestroyInstance();
}

TEST(DX12RHITest, DeviceTest)
{

}

TEST(DX12RHITest, SurfaceTest)
{
	Window window(1920, 1080);

	window.KooriDestroyWindow();
}

TEST(DX12RHITest, SwapChainTest)
{
	Window window(1920, 1080);

	window.KooriDestroyWindow();
}

TEST(DX12RHITest, PipelineTest)
{

}

TEST(DX12RHITest, CommandAllocatorTest)
{

}

TEST(DX12RHITest, RenderLoop)
{
	Window window(1920, 1080);


	window.KooriDestroyWindow();
}