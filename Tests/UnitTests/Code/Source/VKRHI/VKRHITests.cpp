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

#include <Matrix/Matrix4.h>
#include <Vector/Vector3.h>

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
	Matrix4f projection(Matrix4f::Identity);
	projection = projection.PerspectiveProjection(0.01f, 100.f, 16.f / 9.f, 90.f);

	Vector3f position(0.f, 10.f, 0.f);

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
		
		Matrix4f M;
		Matrix4f VP;

		struct
		{
			Matrix4f Model;
			Matrix4f ProjectionView;
		} constants;

		constants.ProjectionView = VP.ViewMatrix(Vector3f(0.f, 0.f, 0.f), Vector3f(0.f, 0.f, -1.f), Vector3f(0.f, 1.f, 0.f));
		constants.ProjectionView = constants.ProjectionView * VP.PerspectiveProjection(0.1f, 100.f, static_cast<float>(window.GetWidth())/static_cast<float>(window.GetHeight()) , 90.f * KM_INTERNAL_DEG_2_RAD);
		constants.Model = M.TRS(M.Translation(0.f, 0.f, 1.f), Matrix4f::Identity, M.Scaling(0.5f, 0.5f, 0.5f)).Transpose();

		cCmdBuffer->BindConstants(&pipeline, 0, sizeof(constants), &constants);

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