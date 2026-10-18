#include <gtest/gtest.h>

#include <Tests/Common.h>

#include <VKRHI/VKInstance.h>
#include <VKRHI/VKDevice.h>
#include <VKRHI/VKSwapChain.h>
#include <VKRHI/VKPipeline.h>
#include <VKRHI/VKPipelineLayout.h>
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

	Window window(1920, 1080, "Vulkan Window");

	VKDevice device;
	device.CreateDevice(&instance);

	VKSwapChain swapChain;
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

TEST(VKRHITest, PipelineTest)
{
	VKInstance instance;
	instance.CreateInstance();

	VKDevice device;
	device.CreateDevice(&instance);

	VKShaderModule vertSM;
	vertSM.CreateShaderModule(&device, IShaderModule::GetShaderData("Assets/Shaders/Unlit/Vulkan/Unlit.vert.spv"));

	VKShaderModule fragSM;
	fragSM.CreateShaderModule(&device, IShaderModule::GetShaderData("Assets/Shaders/Unlit/Vulkan/Unlit.frag.spv"));

	VKPipeline pipeline;
	const IPipeline::GraphicsDescription gPipeline{
		.vertexShader = {
			.shader = &vertSM
		},
		.fragmentShader = {
			.shader = &fragSM
		}
	};
	pipeline.CreatePipeline(&device, gPipeline);
	pipeline.DestroyPipeline(&device);

	fragSM.DestroyShaderModule(&device);
	vertSM.DestroyShaderModule(&device);

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

	std::vector<Vertex> vertices{};
	std::vector<uint16_t> indices{};

	for (const tinyobj::index_t& index : shapes[0].mesh.indices)
	{
		const Vertex v{
			.position = { attrib.vertices[index.vertex_index * 3], attrib.vertices[index.vertex_index * 3 + 1], attrib.vertices[index.vertex_index * 3 + 2] },
			.normal = { attrib.normals[index.normal_index * 3], attrib.normals[index.normal_index * 3 + 1], attrib.normals[index.normal_index * 3 + 2] },
			.textCoords = { attrib.texcoords[index.texcoord_index * 2], attrib.texcoords[index.texcoord_index * 2 + 1] }
		};

		vertices.push_back(v);
		indices.push_back(static_cast<uint16_t>(indices.size()));
	}

	VKInstance instance;
	instance.CreateInstance();

	VKDevice device;
	device.CreateDevice(&instance);

	const IBuffer::Description vertexDesc{
		.usage = IBuffer::Usage::USAGE_VERTEX_BUFFER,
		.size = sizeof(Vertex) * vertices.size(),
		.stride = sizeof(Vertex),
		.data = vertices.data()
	};

	VKBuffer vertexBuffer;
	vertexBuffer.CreateBuffer(&device, vertexDesc);
	vertexBuffer.SetName(&device, "Knight Vertex Buffer");
	vertexBuffer.DestroyBuffer(&device);

	const IBuffer::Description indexDesc{
		.usage = IBuffer::Usage::USAGE_INDEX_BUFFER,
		.size = sizeof(uint16_t) * indices.size(),
		.stride = sizeof(uint16_t),
		.data = indices.data()
	};

	VKBuffer indexBuffer;
	indexBuffer.CreateBuffer(&device, indexDesc);
	indexBuffer.SetName(&device, "Knight Index Buffer");
	indexBuffer.DestroyBuffer(&device);

	device.DestroyDevice();
	instance.DestroyInstance();
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
	graphicsQueue.CreateQueue(&device, IQueue::Type::QUEUE_TYPE_GRAPHICS);

	VKCommandAllocator cmdAlloc;
	cmdAlloc.CreateCommandAllocator(&device);

	ICommandBuffer* cmdBuffer = cmdAlloc.AllocateCommandBuffer(&device);

	const IImage::Description imgDesc{
		.width = static_cast<uint32_t>(width),
		.height = static_cast<uint32_t>(height),
		.channelCount = static_cast<uint8_t>(channels),
		.format = IImage::Format::HONOU_FORMAT_R8G8B8A8_UNORM,
		.usage = IImage::Usage((uint64_t)IImage::Usage::HOUNOU_USAGE_COLOR_ATTACHMENT | (uint64_t)IImage::Usage::HOUNOU_USAGE_SAMPLED | (uint64_t)IImage::Usage::HOUNOU_USAGE_TRANSFER_DST)
	};

	VKImage texture;
	texture.CreateImage(&device, imgDesc);

	const IBuffer::Description stagingDesc{
		.usage = IBuffer::Usage::USAGE_TRANSFER_SRC,
		.size = static_cast<size_t>(width * height * 4),
		.data = data
	};

	VKBuffer stagingBuffer;
	stagingBuffer.CreateBuffer(&device, stagingDesc);

	cmdBuffer->Open();

	const ICommandBuffer::MemoryBarrierInfo info{
		.oldLayout = ICommandBuffer::Layout::UNDEFINED,
		.newLayout = ICommandBuffer::Layout::TRANSFER_DST,

		.syncDest{
			.stages = ICommandBuffer::PipelineStage::TRANSFER,
			.access = ICommandBuffer::Access::TRANSFER_WRITE
		}
	};

	cmdBuffer->AddMemoryBarrier(&texture, info);

	cmdBuffer->Copy(&texture, &stagingBuffer);

	const ICommandBuffer::MemoryBarrierInfo info2{
		.oldLayout = ICommandBuffer::Layout::TRANSFER_DST,
		.newLayout = ICommandBuffer::Layout::SHADER_READ,

		.syncSource{
			.stages = ICommandBuffer::PipelineStage::TRANSFER,
			.access = ICommandBuffer::Access::TRANSFER_WRITE
		},

		.syncDest{
			.stages = ICommandBuffer::PipelineStage::FRAGMENT_SHADER,
			.access = ICommandBuffer::Access::SHADER_READ
		}
	};

	cmdBuffer->AddMemoryBarrier(&texture, info2);

	cmdBuffer->Close();

	device.GetGraphicsQueue()->SubmitCommandBuffer(cmdBuffer);

	device.GetGraphicsQueue()->WaitIdle();

	stagingBuffer.DestroyBuffer(&device);

	texture.DestroyImage(&device);

	stbi_image_free(data);

	cmdAlloc.FreeCommandBuffer(&device, cmdBuffer);

	cmdAlloc.DestroyCommandAllocator(&device);

	device.DestroyDevice();
	instance.DestroyInstance();
}

TEST(VKRHITest, GraphicsRenderLoop)
{
	VKInstance instance;
	instance.CreateInstance();

	bool windowResized = false;
	Window window(1920, 1080, "Vulkan Window");
	window.SetResizeCallback(
		[&](uint32_t _width, uint32_t _height)
		{
			static_cast<void>(_width);
			static_cast<void>(_height);
			windowResized = true;
		}
	);

	VKDevice device;
	device.CreateDevice(&instance);

	VKQueue graphicsQueue;
	graphicsQueue.CreateQueue(&device, IQueue::Type::QUEUE_TYPE_GRAPHICS);

	VKCommandAllocator cmdAlloc;
	cmdAlloc.CreateCommandAllocator(&device);

	std::vector<ICommandBuffer*> cmdBuffers = cmdAlloc.AllocateCommandBuffers(&device, ISwapChain::MAX_FRAMES_IN_FLIGHT + 1);

	VKSwapChain swapChain;
	ISwapChain::Description scDesc{
		.width = window.GetWidth(),
		.height = window.GetHeight(),
		.instance = GetModuleHandle(NULL),
		.hwnd = window.GetHandle()
	};
	swapChain.CreateSwapChain(&instance, &device, scDesc);

	VKImage depthBuffer;
	IImage::Description depth{
		.width = window.GetWidth(),
		.height = window.GetHeight(),
		.channelCount = 4,
		.format = IImage::Format::HOUNOU_FORMAT_D32_FLOAT_S8_UINT,
		.usage = IImage::Usage((uint64_t)IImage::Usage::HOUNOU_USAGE_DEPTH_STENCIL_ATTACHMENT)
	};

	depthBuffer.CreateImage(&device, depth);

	VKShaderModule vertSM;
	vertSM.CreateShaderModule(&device, IShaderModule::GetShaderData("Assets/Shaders/Unlit/Vulkan/Unlit.vert.spv"));

	VKShaderModule fragSM;
	fragSM.CreateShaderModule(&device, IShaderModule::GetShaderData("Assets/Shaders/Unlit/Vulkan/Unlit.frag.spv"));

	VKPipeline pipeline;
	const IPipeline::GraphicsDescription gPipeline{
		.vertexShader = {
			.shader = &vertSM
		},
		.fragmentShader = {
			.shader = &fragSM
		}
	};
	pipeline.CreatePipeline(&device, gPipeline);

	fragSM.DestroyShaderModule(&device);
	vertSM.DestroyShaderModule(&device);

	tinyobj::attrib_t attrib{};
	std::vector<tinyobj::shape_t> shapes{};
	std::vector<tinyobj::material_t> materials{};
	tinyobj::LoadObj(&attrib, &shapes, &materials, nullptr, nullptr, "Assets/Meshes/Knight.obj");

	std::vector<Vertex> vertices{};
	std::vector<uint16_t> indices{};

	for (const tinyobj::index_t& index : shapes[0].mesh.indices)
	{
		const Vertex v{
			.position = { attrib.vertices[index.vertex_index * 3], attrib.vertices[index.vertex_index * 3 + 1], attrib.vertices[index.vertex_index * 3 + 2] },
			.normal = { attrib.normals[index.normal_index * 3], attrib.normals[index.normal_index * 3 + 1], attrib.normals[index.normal_index * 3 + 2] },
			.textCoords = { attrib.texcoords[index.texcoord_index * 2], attrib.texcoords[index.texcoord_index * 2 + 1] }
		};

		vertices.push_back(v);
		indices.push_back(static_cast<uint16_t>(indices.size()));
	}

	const IBuffer::Description vertexDesc{
		.usage = IBuffer::Usage::USAGE_VERTEX_BUFFER,
		.size = sizeof(Vertex) * vertices.size(),
		.data = vertices.data()
	};

	VKBuffer vertexBuffer;
	vertexBuffer.CreateBuffer(&device, vertexDesc);
	vertexBuffer.SetName(&device, "Knight Vertex Buffer");

	const IBuffer::Description indexDesc{
		.usage = IBuffer::Usage::USAGE_INDEX_BUFFER,
		.size = sizeof(uint16_t) * indices.size(),
		.data = indices.data()
	};

	VKBuffer indexBuffer;
	indexBuffer.CreateBuffer(&device, indexDesc);
	indexBuffer.SetName(&device, "Knight Index Buffer");

	stbi_set_flip_vertically_on_load(true);

	int width, height, channels;
	unsigned char* data = stbi_load("Assets/Textures/Knight.png", &width, &height, &channels, STBI_rgb_alpha);

	if (!data) return;

	const IImage::Description imgDesc{
		.width = static_cast<uint32_t>(width),
		.height = static_cast<uint32_t>(height),
		.channelCount = static_cast<uint8_t>(channels),
		.format = IImage::Format::HONOU_FORMAT_R8G8B8A8_UNORM,
		.usage = IImage::Usage((uint64_t)IImage::Usage::HOUNOU_USAGE_COLOR_ATTACHMENT | (uint64_t)IImage::Usage::HOUNOU_USAGE_SAMPLED | (uint64_t)IImage::Usage::HOUNOU_USAGE_TRANSFER_DST)
	};

	VKImage texture;
	texture.CreateImage(&device, imgDesc);

	const IBuffer::Description stagingDesc{
		.usage = IBuffer::Usage::USAGE_TRANSFER_SRC,
		.size = static_cast<size_t>(width * height * 4),
		.data = data
	};

	VKBuffer stagingBuffer;
	stagingBuffer.CreateBuffer(&device, stagingDesc);

	cmdBuffers[ISwapChain::MAX_FRAMES_IN_FLIGHT]->Reset();
	cmdBuffers[ISwapChain::MAX_FRAMES_IN_FLIGHT]->Open();

	const ICommandBuffer::MemoryBarrierInfo infoIm{
		.oldLayout = ICommandBuffer::Layout::UNDEFINED,
		.newLayout = ICommandBuffer::Layout::TRANSFER_DST,

		.syncDest{
			.stages = ICommandBuffer::PipelineStage::TRANSFER,
			.access = ICommandBuffer::Access::TRANSFER_WRITE
		}
	};

	cmdBuffers[ISwapChain::MAX_FRAMES_IN_FLIGHT]->AddMemoryBarrier(&texture, infoIm);

	cmdBuffers[ISwapChain::MAX_FRAMES_IN_FLIGHT]->Copy(&texture, &stagingBuffer);

	const ICommandBuffer::MemoryBarrierInfo infoIm2{
		.oldLayout = ICommandBuffer::Layout::TRANSFER_DST,
		.newLayout = ICommandBuffer::Layout::SHADER_READ,

		.syncSource{
			.stages = ICommandBuffer::PipelineStage::TRANSFER,
			.access = ICommandBuffer::Access::TRANSFER_WRITE
		},

		.syncDest{
			.stages = ICommandBuffer::PipelineStage::FRAGMENT_SHADER,
			.access = ICommandBuffer::Access::SHADER_READ
		}
	};

	cmdBuffers[ISwapChain::MAX_FRAMES_IN_FLIGHT]->AddMemoryBarrier(&texture, infoIm2);
	cmdBuffers[ISwapChain::MAX_FRAMES_IN_FLIGHT]->Close();

	graphicsQueue.SubmitCommandBuffer(cmdBuffers[ISwapChain::MAX_FRAMES_IN_FLIGHT]);

	graphicsQueue.WaitIdle();

	stagingBuffer.DestroyBuffer(&device);

	stbi_image_free(data);

	VKSampler sampler;
	const ISampler::Description sInfo{};
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

		cCmdBuffer->AddLabel("Render Frame", { 0.0, 1.0, 0.0, 1.0 });

		const ICommandBuffer::MemoryBarrierInfo barrierInfo{
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

		cCmdBuffer->AddMemoryBarrier(swapChain.GetCurrentImage(), barrierInfo);

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

		cCmdBuffer->AddLabel("Pipeline", { 0.0, 0.0, 1.0, 1.0 });

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
		constants.ProjectionView = constants.ProjectionView * VP.PerspectiveProjection(0.1f, 100.f, static_cast<float>(window.GetWidth()) / static_cast<float>(window.GetHeight()), 90.f * KM_DEG_2_RAD(float));
		constants.Model = M.TRS(M.Translation(0.f, 2.5f, -5.f), M.XRotation(-180.f * KM_DEG_2_RAD(float)) * M.YRotation(rotation * KM_DEG_2_RAD(float)), M.Scaling(1.f, 1.f, 1.f)).Transpose();
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

		cCmdBuffer->DrawIndexed(static_cast<uint32_t>(indices.size()));

		cCmdBuffer->EndRendering();

		cCmdBuffer->EndLabel();

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

		cCmdBuffer->EndLabel();

		cCmdBuffer->Close();

		graphicsQueue.SubmitCommandBuffer(cCmdBuffer, swapChain.GetFence(frameIndex), swapChain.GetImageSyncPoint(frameIndex), swapChain.GetRenderSyncPoint());

		frameIndex = (frameIndex + 1) % ISwapChain::MAX_FRAMES_IN_FLIGHT;

		swapChain.Present(&device, &graphicsQueue, swapChain.GetRenderSyncPoint());

		window.UpdateWindow();

		if (windowResized)
		{
			windowResized = false;

			device.WaitIdle();
			scDesc = {
				.width = window.GetWidth(),
				.height = window.GetHeight()
			};
			swapChain.RecreateSwapChain(&device, scDesc);
			depthBuffer.DestroyImage(&device);

			depth.width = window.GetWidth();
			depth.height = window.GetHeight();

			depthBuffer.CreateImage(&device, depth);
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

	swapChain.DestroySwapChain(&instance, &device);

	cmdAlloc.FreeCommandBuffers(&device, cmdBuffers);
	cmdAlloc.DestroyCommandAllocator(&device);

	device.DestroyDevice();
	instance.DestroyInstance();

	window.KooriDestroyWindow();
}

TEST(VKRHITest, ComputeRenderLoop)
{
	VKInstance instance;
	instance.CreateInstance();

	bool windowResized = false;
	Window window(1920, 1080, "Vulkan Window");
	window.SetResizeCallback(
		[&](uint32_t _width, uint32_t _height)
		{
			static_cast<void>(_width);
			static_cast<void>(_height);
			windowResized = true;
		}
	);

	VKDevice device;
	device.CreateDevice(&instance);

	VKQueue graphicsQueue;
	graphicsQueue.CreateQueue(&device, IQueue::Type::QUEUE_TYPE_GRAPHICS);

	VKCommandAllocator cmdAlloc;
	cmdAlloc.CreateCommandAllocator(&device);

	std::vector<ICommandBuffer*> cmdBuffers = cmdAlloc.AllocateCommandBuffers(&device, ISwapChain::MAX_FRAMES_IN_FLIGHT);

	VKSwapChain swapChain;
	ISwapChain::Description scDesc{
		.width = window.GetWidth(),
		.height = window.GetHeight(),
		.instance = GetModuleHandle(NULL),
		.hwnd = window.GetHandle()
	};
	swapChain.CreateSwapChain(&instance, &device, scDesc);

	VKShaderModule computeSM;
	computeSM.CreateShaderModule(&device, IShaderModule::GetShaderData("Assets/Shaders/HelloTriangle/HelloTriangle.comp.spv"));

	VKPipeline pipeline;
	const IPipeline::ComputeDescription cPipeline{
		.computeShader = {
			.shader = &computeSM
		}
	};
	pipeline.CreatePipeline(&device, cPipeline);

	computeSM.DestroyShaderModule(&device);

	VKDescriptorAllocator descAllocator;
	descAllocator.CreateDescriptorAllocator(&device);

	IImage::Description computeImageDesc{
		.width = window.GetWidth(),
		.height = window.GetHeight(),
		.channelCount = 4,
		.format = IImage::Format::HONOU_FORMAT_R8G8B8A8_UNORM,
		.usage = IImage::Usage((uint64_t)IImage::Usage::HOUNOU_USAGE_COLOR_ATTACHMENT |
			(uint64_t)IImage::Usage::HOUNOU_USAGE_SAMPLED |
			(uint64_t)IImage::Usage::HOUNOU_USAGE_TRANSFER_DST |
			(uint64_t)IImage::Usage::HOUNOU_USAGE_TRANSFER_SRC |
			(uint64_t)IImage::Usage::HOUNOU_USAGE_STORAGE)
	};

	std::vector<VKImage> computeImages(ISwapChain::MAX_FRAMES_IN_FLIGHT);
	for (int i = 0; i < ISwapChain::MAX_FRAMES_IN_FLIGHT; ++i)
	{
		computeImages[i].CreateImage(&device, computeImageDesc);
	}

	std::vector<IDescriptor*> descs = descAllocator.AllocateDescriptors(&device, &pipeline, ISwapChain::MAX_FRAMES_IN_FLIGHT);

	uint32_t frameIndex = 0;

	while (!window.WindowShouldClose())
	{
		swapChain.GetNextSwapChainImage(&device, frameIndex);

		ICommandBuffer* cCmdBuffer = cmdBuffers[frameIndex];

		cCmdBuffer->Reset();

		cCmdBuffer->Open();

		ICommandBuffer::MemoryBarrierInfo barrierInfo{
			.oldLayout = ICommandBuffer::Layout::UNDEFINED,
			.newLayout = ICommandBuffer::Layout::UNORDERED_ACCESS,

			.syncSource{
				.stages = ICommandBuffer::PipelineStage::NONE,
				.access = ICommandBuffer::Access::NONE
			},

			.syncDest{
				.stages = ICommandBuffer::PipelineStage::COMPUTE_SHADER,
				.access = ICommandBuffer::Access::SHADER_WRITE
			}
		};

		cCmdBuffer->AddMemoryBarrier(&computeImages[frameIndex], barrierInfo);

		cCmdBuffer->BindPipeline(&pipeline);

		const VkDescriptorImageInfo imageInfo{
			.imageView = computeImages[frameIndex].CastToVK()->GetRawImageView(),
			.imageLayout = VK_IMAGE_LAYOUT_GENERAL
		};

		const std::array<VkWriteDescriptorSet, 1> descriptorWrites{
			VkWriteDescriptorSet{
				.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
				.dstSet = descs[frameIndex]->CastToVK()->GetRaw(),
				.dstBinding = 0,
				.dstArrayElement = 0,
				.descriptorCount = 1,
				.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,
				.pImageInfo = &imageInfo
			}
		};

		vkUpdateDescriptorSets(device.CastToVK()->GetLogicalDevice(), 1, descriptorWrites.data(), 0, nullptr);

		cCmdBuffer->BindDescriptor(&pipeline, descs[frameIndex], 0);

		cCmdBuffer->Dispatch((window.GetWidth() + 15) / 16, (window.GetHeight() + 15) / 16, 1);

		barrierInfo = {
			.oldLayout = ICommandBuffer::Layout::UNORDERED_ACCESS,
			.newLayout = ICommandBuffer::Layout::TRANSFER_SRC,

			.syncSource{
				.stages = ICommandBuffer::PipelineStage::COMPUTE_SHADER,
				.access = ICommandBuffer::Access::SHADER_WRITE
			},

			.syncDest{
				.stages = ICommandBuffer::PipelineStage::TRANSFER,
				.access = ICommandBuffer::Access::TRANSFER_READ
			}
		};

		cCmdBuffer->AddMemoryBarrier(&computeImages[frameIndex], barrierInfo);

		barrierInfo = {
			.oldLayout = ICommandBuffer::Layout::UNDEFINED,
			.newLayout = ICommandBuffer::Layout::TRANSFER_DST,

			.syncSource{
				.stages = ICommandBuffer::PipelineStage::NONE,
				.access = ICommandBuffer::Access::NONE
			},

			.syncDest{
				.stages = ICommandBuffer::PipelineStage::TRANSFER,
				.access = ICommandBuffer::Access::TRANSFER_WRITE
			}
		};

		cCmdBuffer->AddMemoryBarrier(swapChain.GetCurrentImage(), barrierInfo);

		cCmdBuffer->Copy(swapChain.GetCurrentImage(), &computeImages[frameIndex]);

		barrierInfo = {
			.oldLayout = ICommandBuffer::Layout::TRANSFER_DST,
			.newLayout = ICommandBuffer::Layout::PRESENT,

			.syncSource{
				.stages = ICommandBuffer::PipelineStage::TRANSFER,
				.access = ICommandBuffer::Access::TRANSFER_WRITE
			},

			.syncDest{
				.stages = ICommandBuffer::PipelineStage::NONE,
				.access = ICommandBuffer::Access::NONE
			}
		};

		cCmdBuffer->AddMemoryBarrier(swapChain.GetCurrentImage(), barrierInfo);

		cCmdBuffer->Close();

		graphicsQueue.SubmitCommandBuffer(cCmdBuffer, swapChain.GetFence(frameIndex), swapChain.GetImageSyncPoint(frameIndex), swapChain.GetRenderSyncPoint());

		frameIndex = (frameIndex + 1) % ISwapChain::MAX_FRAMES_IN_FLIGHT;

		swapChain.Present(&device, &graphicsQueue, swapChain.GetRenderSyncPoint());

		window.UpdateWindow();

		if (windowResized)
		{
			windowResized = false;

			device.WaitIdle();

			for (int i = 0; i < ISwapChain::MAX_FRAMES_IN_FLIGHT; ++i)
			{
				computeImages[i].DestroyImage(&device);
				computeImageDesc.width = window.GetWidth();
				computeImageDesc.height = window.GetHeight();
				computeImages[i].CreateImage(&device, computeImageDesc);
			}

			scDesc = {
				.width = window.GetWidth(),
				.height = window.GetHeight()
			};
			swapChain.RecreateSwapChain(&device, scDesc);
		}
	}

	graphicsQueue.WaitIdle();
	// or device.WaitIdle();

	for (int i = 0; i < ISwapChain::MAX_FRAMES_IN_FLIGHT; ++i)
	{
		computeImages[i].DestroyImage(&device);
	}

	descAllocator.DestroyDescriptorAllocator(&device);

	pipeline.DestroyPipeline(&device);

	swapChain.DestroySwapChain(&instance, &device);

	cmdAlloc.FreeCommandBuffers(&device, cmdBuffers);
	cmdAlloc.DestroyCommandAllocator(&device);

	device.DestroyDevice();
	instance.DestroyInstance();

	window.KooriDestroyWindow();
}
