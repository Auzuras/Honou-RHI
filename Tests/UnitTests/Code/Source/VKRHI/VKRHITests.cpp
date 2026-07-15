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
#include <VKRHI/VKShaderModule.h>
#include <VKRHI/VKSyncPoint.h>
#include <VKRHI/VKQueue.h>

#include <tiny_obj_loader.h>
#include <Helpers/Vertex.h>

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
	swapChain.CreateSwapChain(&device, &surface, window.GetWidth(), window.GetHeight());
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

TEST(VKRHITest, Mesh)
{
	tinyobj::attrib_t attrib{};
	std::vector<tinyobj::shape_t> shapes{};
	std::vector<tinyobj::material_t> materials{};
	tinyobj::LoadObj(&attrib, &shapes, &materials, nullptr, nullptr, "Assets/Meshes/Sphere.obj");

	const size_t indicesCount = shapes[0].mesh.indices.size();
	std::vector<Vertex> vertices{};
	std::vector<uint16_t> indices{};

	for (const tinyobj::index_t& index : shapes[0].mesh.indices)
	{
		const Vertex v{
			.position	=	{ attrib.vertices[index.vertex_index * 3], attrib.vertices[index.vertex_index * 3 + 1], attrib.vertices[index.vertex_index * 3 + 2] }
		};

		vertices.push_back(v);
		indices.push_back(indices.size());
	}

	VKInstance instance;
	instance.CreateInstance();

	VKDevice device;
	device.CreateDevice(&instance);

	VKBuffer vertexBuffer;
	vertexBuffer.CreateBuffer(&device, vertices.data(), sizeof(Vertex) * vertices.size(), IBuffer::BufferUsage::USAGE_VERTEX_BUFFER);
	vertexBuffer.DestroyBuffer(&device);

	VKBuffer indexBuffer;
	indexBuffer.CreateBuffer(&device, indices.data(), sizeof(uint16_t) * indices.size(), IBuffer::BufferUsage::USAGE_INDEX_BUFFER);
	indexBuffer.DestroyBuffer(&device);
}

TEST(VKRHITest, RenderLoop)
{
	VKInstance instance;
	instance.CreateInstance();

	bool windowResized = false;
	Window window(1920, 1080);
	window.SetResizeCallback(
		[&](uint32_t _width, uint32_t _height)
		{
			windowResized = true;
		}
	);

	VKSurface surface;
	ISurface::SurfaceInformation info{
		.instance = GetModuleHandle(NULL),
		.hwnd = window.GetHandle()
	};

	surface.CreateSurface(&instance, info);

	VKDevice device;
	device.CreateDevice(&instance);

	VKQueue graphicsQueue;
	graphicsQueue.CreateQueue(&device);

	VKCommandAllocator cmdAlloc;
	cmdAlloc.CreateCommandAllocator(&device);

	std::vector<ICommandBuffer*> cmdBuffers = cmdAlloc.AllocateCommandBuffers(&device, ISwapChain::MAX_FRAMES_IN_FLIGHT);

	VKSwapChain swapChain;
	swapChain.CreateSwapChain(&device, &surface, window.GetWidth(), window.GetHeight());

	VKPipeline pipeline;
	pipeline.CreatePipeline(&device);

	tinyobj::attrib_t attrib{};
	std::vector<tinyobj::shape_t> shapes{};
	std::vector<tinyobj::material_t> materials{};
	tinyobj::LoadObj(&attrib, &shapes, &materials, nullptr, nullptr, "Assets/Meshes/Sphere.obj");

	const size_t indicesCount = shapes[0].mesh.indices.size();
	std::vector<Vertex> vertices{};
	std::vector<uint16_t> indices{};

	for (const tinyobj::index_t& index : shapes[0].mesh.indices)
	{
		const Vertex v{
			.position = { attrib.vertices[index.vertex_index * 3], attrib.vertices[index.vertex_index * 3 + 1], attrib.vertices[index.vertex_index * 3 + 2] }
		};

		vertices.push_back(v);
		indices.push_back(indices.size());
	}

	VKBuffer vertexBuffer;
	vertexBuffer.CreateBuffer(&device, vertices.data(), sizeof(Vertex) * vertices.size(), IBuffer::BufferUsage::USAGE_VERTEX_BUFFER);

	VKBuffer indexBuffer;
	indexBuffer.CreateBuffer(&device, indices.data(), sizeof(uint16_t) * indices.size(), IBuffer::BufferUsage::USAGE_INDEX_BUFFER);

	uint32_t frameIndex = 0;

	while (!window.WindowShouldClose())
	{
		swapChain.GetNextSwapChainImage(&device, frameIndex);

		ICommandBuffer* cCmdBuffer = cmdBuffers[frameIndex];

		cCmdBuffer->Reset();

		cCmdBuffer->Open();

		cCmdBuffer->AddMemoryBarrier0(swapChain.GetCurrentImage());

		cCmdBuffer->BeginRendering(swapChain.GetCurrentImage(), window.GetWidth(), window.GetHeight());

		cCmdBuffer->SetViewport(window.GetWidth(), window.GetHeight());

		cCmdBuffer->SetScissor(window.GetWidth(), window.GetHeight());

		cCmdBuffer->BindPipeline(&pipeline);

		cCmdBuffer->BindVertexBuffer(&vertexBuffer);

		cCmdBuffer->BindIndexBuffer(&indexBuffer);
		
		cCmdBuffer->DrawIndexed(indices.size());

		cCmdBuffer->EndRendering();

		cCmdBuffer->AddMemoryBarrier1(swapChain.GetCurrentImage());

		cCmdBuffer->Close();

		graphicsQueue.SubmitCommandBuffer(cCmdBuffer, swapChain.GetFence(frameIndex), swapChain.GetImageSyncPoint(frameIndex), swapChain.GetRenderSyncPoint());

		frameIndex = (frameIndex + 1) % ISwapChain::MAX_FRAMES_IN_FLIGHT;

		swapChain.Present(&device, &graphicsQueue, swapChain.GetRenderSyncPoint());

		window.UpdateWindow();

		if (windowResized)
		{
			windowResized = false;

			device.WaitIdle();
			swapChain.RecreateSwapChain(&device, &surface, window.GetWidth(), window.GetHeight());
		}
	}

	graphicsQueue.WaitIdle();
	// or device.WaitIdle();

	vertexBuffer.DestroyBuffer(&device);
	indexBuffer.DestroyBuffer(&device);

	pipeline.DestroyPipeline(&device);
	
	swapChain.DestroySwapChain(&device);

	cmdAlloc.FreeCommandBuffers(&device, cmdBuffers);
	cmdAlloc.DestroyCommandAllocator(&device);

	device.DestroyDevice();
	surface.DestroySurface(&instance);
	instance.DestroyInstance();

	window.KooriDestroyWindow();
}