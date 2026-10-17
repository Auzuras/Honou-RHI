#include <gtest/gtest.h>

#include <Window/Window.h>

#include <DX12RHI/DX12Instance.h>
#include <DX12RHI/DX12Device.h>
#include <DX12RHI/DX12SwapChain.h>
#include <DX12RHI/DX12Queue.h>
#include <DX12RHI/DX12Pipeline.h>
#include <DX12RHI/DX12ShaderModule.h>

class DX12RHITest : public testing::Test {};

TEST(DX12RHITest, InstanceTest)
{
	DX12Instance instance;
	instance.CreateInstance();
	instance.DestroyInstance();
}

TEST(DX12RHITest, DeviceTest)
{
	DX12Instance instance;
	instance.CreateInstance();

	DX12Device device;
	device.CreateDevice(&instance);
	device.DestroyDevice();

	instance.DestroyInstance();
}

TEST(DX12RHITest, SwapChainTest)
{
	DX12Instance instance;
	instance.CreateInstance();

	Window window(1920, 1080, "DX12 Window");

	DX12Device device;
	device.CreateDevice(&instance);

	DX12Queue graphicsQueue;
	graphicsQueue.CreateQueue(&device, IQueue::Type::QUEUE_TYPE_GRAPHICS);

	DX12SwapChain swapChain;
	const ISwapChain::Description scDesc{
		.width = window.GetWidth(),
		.height = window.GetHeight(),
		.instance = GetModuleHandle(NULL),
		.hwnd = window.GetHandle()
	};
	swapChain.CreateSwapChain(&instance, &device, scDesc);
	swapChain.DestroySwapChain(&instance, &device);

	device.DestroyDevice();
	instance.DestroyInstance();

	window.KooriDestroyWindow();
}

TEST(DX12RHITest, PipelineTest)
{
	DX12Instance instance;
	instance.CreateInstance();

	DX12Device device;
	device.CreateDevice(&instance);

	DX12ShaderModule vertSM;
	vertSM.CreateShaderModule(&device, IShaderModule::GetShaderData("Assets/Shaders/Unlit/DX12/Unlit_VERT.cso"));

	DX12ShaderModule pixSM;
	pixSM.CreateShaderModule(&device, IShaderModule::GetShaderData("Assets/Shaders/Unlit/DX12/Unlit_PIX.cso"));

	DX12Pipeline pipeline;
	const IPipeline::GraphicsDescription gPipeline{
		.vertexShader = {
			.shader = &vertSM
		},
		.fragmentShader = {
			.shader = &pixSM
		}
	};
	pipeline.CreatePipeline(&device, gPipeline);
	pipeline.DestroyPipeline(&device);

	pixSM.DestroyShaderModule(&device);
	vertSM.DestroyShaderModule(&device);

	device.DestroyDevice();
	instance.DestroyInstance();
}

TEST(DX12RHITest, CommandAllocatorTest)
{
	DX12Instance instance;
	instance.CreateInstance();

	DX12Device device;
	device.CreateDevice(&instance);

	DX12CommandAllocator cmdAlloc;
	cmdAlloc.CreateCommandAllocator(&device);

	ICommandBuffer* cmdBuffer = cmdAlloc.AllocateCommandBuffer(&device);
	cmdAlloc.FreeCommandBuffer(&device, cmdBuffer);

	cmdAlloc.DestroyCommandAllocator(&device);

	device.DestroyDevice();
	instance.DestroyInstance();
}

TEST(DX12RHITest, RenderLoop)
{
	Window window(1920, 1080, "DX12 Window");


	window.KooriDestroyWindow();
}