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
#include <VKRHI/VKSampler.h>
#include <VKRHI/VKDescriptorAllocator.h>
#include <VKRHI/VKDescriptor.h>

#include <tiny_obj_loader.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <Helpers/Vertex.h>

#include <Window/Window.h>

#include <Matrix/Matrix4.h>
#include <Vector/Vector3.h>
#include <chrono>

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
	tinyobj::LoadObj(&attrib, &shapes, &materials, nullptr, nullptr, "Assets/Meshes/Knight.obj");

	const size_t indicesCount = shapes[0].mesh.indices.size();
	std::vector<Vertex> vertices{};
	std::vector<uint16_t> indices{};

	for (const tinyobj::index_t& index : shapes[0].mesh.indices)
	{
		const Vertex v{
			.position	=	{ attrib.vertices[index.vertex_index * 3], attrib.vertices[index.vertex_index * 3 + 1], attrib.vertices[index.vertex_index * 3 + 2] },
			.normal		=	{ attrib.normals[index.normal_index * 3], attrib.normals[index.normal_index * 3 + 1], attrib.normals[index.normal_index * 3 + 2] },
			.textCoords	=	{ attrib.texcoords[index.texcoord_index * 2], attrib.texcoords[index.texcoord_index * 2 + 1] }
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

TEST(VKRHITest, Texture)
{
	int width, height, channels;
	unsigned char* data = stbi_load("Assets/Textures/Knight.png", &width, &height, &channels, STBI_rgb_alpha);

	if (!data) return;

	VKInstance instance;
	instance.CreateInstance();

	VKDevice device;
	device.CreateDevice(&instance);

	VKQueue graphicsQueue;
	graphicsQueue.CreateQueue(&device);

	const ImageDescription imgDesc{
		.width = static_cast<uint32_t>(width),
		.height = static_cast<uint32_t>(height),
		.channelCount = static_cast<uint8_t>(channels),
		.data = data,
		.format = TextureFormat::HONOU_FORMAT_R8G8B8A8_SRGB
	};

	VKImage texture;
	texture.CreateImage(&device, imgDesc);
	texture.DestroyImage(&device);

	stbi_image_free(data);
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

	VKImage depthBuffer;
	ImageDescription depth{
		.width = window.GetWidth(),
		.height = window.GetHeight(),
		.channelCount = 4,
		.data = nullptr,
		.format = TextureFormat::HOUNOU_FORMAT_D32_FLOAT_S8_UINT
	};
	depthBuffer.CreateDepthImage(&device, depth);

	VKPipeline pipeline;
	pipeline.CreatePipeline(&device);

	tinyobj::attrib_t attrib{};
	std::vector<tinyobj::shape_t> shapes{};
	std::vector<tinyobj::material_t> materials{};
	tinyobj::LoadObj(&attrib, &shapes, &materials, nullptr, nullptr, "Assets/Meshes/Knight.obj");

	const size_t indicesCount = shapes[0].mesh.indices.size();
	std::vector<Vertex> vertices{};
	std::vector<uint16_t> indices{};

	for (const tinyobj::index_t& index : shapes[0].mesh.indices)
	{
		const Vertex v{
			.position	=	{ attrib.vertices[index.vertex_index * 3], attrib.vertices[index.vertex_index * 3 + 1], attrib.vertices[index.vertex_index * 3 + 2] },
			.normal		=	{ attrib.normals[index.normal_index * 3], attrib.normals[index.normal_index * 3 + 1], attrib.normals[index.normal_index * 3 + 2] },
			.textCoords	=	{ attrib.texcoords[index.texcoord_index * 2], attrib.texcoords[index.texcoord_index * 2 + 1] }
		};

		vertices.push_back(v);
		indices.push_back(indices.size());
	}

	VKBuffer vertexBuffer;
	vertexBuffer.CreateBuffer(&device, vertices.data(), sizeof(Vertex) * vertices.size(), IBuffer::BufferUsage::USAGE_VERTEX_BUFFER);

	VKBuffer indexBuffer;
	indexBuffer.CreateBuffer(&device, indices.data(), sizeof(uint16_t) * indices.size(), IBuffer::BufferUsage::USAGE_INDEX_BUFFER);

	stbi_set_flip_vertically_on_load(true);

	int width, height, channels;
	unsigned char* data = stbi_load("Assets/Textures/Knight.png", &width, &height, &channels, STBI_rgb_alpha);

	if (!data) return;

	const ImageDescription imgDesc{
		.width = static_cast<uint32_t>(width),
		.height = static_cast<uint32_t>(height),
		.channelCount = static_cast<uint8_t>(channels),
		.data = data,
		.format = TextureFormat::HONOU_FORMAT_R8G8B8A8_UNORM
	};

	VKImage texture;
	texture.CreateImage(&device, imgDesc);

	stbi_image_free(data);

	VKSampler sampler;
	const ISampler::SamplerInfo sInfo{};
	sampler.CreateSampler(&device, sInfo);

	VKDescriptorAllocator descAllocator;
	descAllocator.CreateDescriptorAllocator(&device);

	std::vector<IDescriptor*> descs = descAllocator.AllocateDescriptors(&device, &pipeline, ISwapChain::MAX_FRAMES_IN_FLIGHT);

	uint32_t frameIndex = 0;
	
	float rotation = 0.f;

	auto lastTime = std::chrono::high_resolution_clock::now();
	while (!window.WindowShouldClose())
	{
		auto currentTime = std::chrono::high_resolution_clock::now();
		float dt = std::chrono::duration<float>(currentTime - lastTime).count();
		lastTime = currentTime;

		swapChain.GetNextSwapChainImage(&device, frameIndex);

		ICommandBuffer* cCmdBuffer = cmdBuffers[frameIndex];

		cCmdBuffer->Reset();

		cCmdBuffer->Open();

		const ICommandBuffer::MemoryBarrierInfo info{
			.oldLayout = ICommandBuffer::Layout::UNDEFINED,
			.newLayout = ICommandBuffer::Layout::COLOR_OPTIMAL,

			.syncSource{
				.stages = ICommandBuffer::PipelineStage::COLOR_ATTACHMENT,
				.access = ICommandBuffer::Access::NONE
			},

			.syncDest{
				.stages = ICommandBuffer::PipelineStage::COLOR_ATTACHMENT,
				.access = (ICommandBuffer::Access)((uint64_t)ICommandBuffer::Access::COLOR_ATTACHMENT_READ | (uint64_t)ICommandBuffer::Access::COLOR_ATTACHMENT_WRITE)
			}
		};

		cCmdBuffer->AddMemoryBarrier(swapChain.GetCurrentImage(), info);

		const ICommandBuffer::MemoryBarrierInfo infoDepth{
			.oldLayout = ICommandBuffer::Layout::UNDEFINED,
			.newLayout = ICommandBuffer::Layout::DEPTH_STENCIL_OPTIMAL,

			.syncSource{
				.stages = ICommandBuffer::PipelineStage::EARLY_FRAGMENT_TEST,
				.access = ICommandBuffer::Access::DEPTH_STENCIL_ATTACHMENT_WRITE
			},

			.syncDest{
				.stages = ICommandBuffer::PipelineStage::LATE_FRAGMENT_TEST,
				.access = ICommandBuffer::Access::DEPTH_STENCIL_ATTACHMENT_WRITE
			}
		};

		cCmdBuffer->AddMemoryBarrier(&depthBuffer, infoDepth);

		cCmdBuffer->BeginRendering(swapChain.GetCurrentImage(), &depthBuffer, window.GetWidth(), window.GetHeight());

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

		constants.ProjectionView = VP.ViewMatrix(Vector3f(0.f, 0.f, 0.f), Vector3f(0.f, 0.f, 1.f), Vector3f(0.f, 1.f, 0.f));
		constants.ProjectionView = constants.ProjectionView * VP.PerspectiveProjection(0.1f, 100.f, static_cast<float>(window.GetWidth())/static_cast<float>(window.GetHeight()) , 90.f * KM_INTERNAL_DEG_2_RAD);
		constants.Model = M.TRS(M.Translation(0.f, 2.5f, -5.f), M.XRotation(-180.f * KM_INTERNAL_DEG_2_RAD) * M.YRotation(rotation * KM_INTERNAL_DEG_2_RAD), M.Scaling(1.f, 1.f, 1.f)).Transpose();
		rotation += 25.f * dt;

		cCmdBuffer->BindConstants(&pipeline, 0, sizeof(constants), &constants);

		const VkDescriptorImageInfo samplerInfo{
			.sampler = sampler.CastToVK()->GetRaw()
		};

		const VkDescriptorImageInfo imageInfo{
			.imageView = texture.CastToVK()->GetRawImageView(),
			.imageLayout = VK_IMAGE_LAYOUT_READ_ONLY_OPTIMAL
		};

		const std::array<VkWriteDescriptorSet, 2> descriptorWrites{
			VkWriteDescriptorSet{
				.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
				.dstSet = descs[frameIndex]->CastToVK()->GetRaw(),
				.dstBinding = 0,
				.dstArrayElement = 0,
				.descriptorCount = 1,
				.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLER,
				.pImageInfo = &samplerInfo
			},
			VkWriteDescriptorSet{
				.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
				.dstSet = descs[frameIndex]->CastToVK()->GetRaw(),
				.dstBinding = 1,
				.dstArrayElement = 0,
				.descriptorCount = 1,
				.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE,
				.pImageInfo = &imageInfo
			}
		};

		vkUpdateDescriptorSets(device.CastToVK()->GetLogicalDevice(), 2, descriptorWrites.data(), 0, nullptr);

		cCmdBuffer->BindDescriptor(&pipeline, descs[frameIndex], 0);

		cCmdBuffer->DrawIndexed(indices.size());

		cCmdBuffer->EndRendering();

		const ICommandBuffer::MemoryBarrierInfo info2{
			.oldLayout = ICommandBuffer::Layout::COLOR_OPTIMAL,
			.newLayout = ICommandBuffer::Layout::PRESENT,

			.syncSource{
				.stages = ICommandBuffer::PipelineStage::COLOR_ATTACHMENT,
				.access = ICommandBuffer::Access::NONE
			},

			.syncDest{
				.stages = ICommandBuffer::PipelineStage::COLOR_ATTACHMENT,
				.access = ICommandBuffer::Access::COLOR_ATTACHMENT_WRITE
			}
		};

		cCmdBuffer->AddMemoryBarrier(swapChain.GetCurrentImage(), info2);

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
			depthBuffer.DestroyImage(&device);

			depth.width = window.GetWidth();
			depth.height = window.GetHeight();

			depthBuffer.CreateDepthImage(&device, depth);
		}
	}

	graphicsQueue.WaitIdle();
	// or device.WaitIdle();

	depthBuffer.DestroyImage(&device);

	sampler.DestroySampler(&device);

	texture.DestroyImage(&device);

	descAllocator.DestroyDescriptorAllocator(&device);

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
