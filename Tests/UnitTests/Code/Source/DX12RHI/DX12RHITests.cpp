#include <gtest/gtest.h>

#include <Tests/Common.h>

#include <DX12RHI/DX12Instance.h>
#include <DX12RHI/DX12Device.h>
#include <DX12RHI/DX12SwapChain.h>
#include <DX12RHI/DX12Pipeline.h>
//#include <DX12RHI/DX12PipelineLayout.h>
#include <DX12RHI/DX12CommandAllocator.h>
#include <DX12RHI/DX12CommandBuffer.h>
#include <DX12RHI/DX12Image.h>
#include <DX12RHI/DX12Buffer.h>
#include <DX12RHI/DX12ShaderModule.h>
//#include <DX12RHI/DX12SyncPoint.h>
#include <DX12RHI/DX12Queue.h>
//#include <DX12RHI/DX12Sampler.h>
//#include <DX12RHI/DX12DescriptorAllocator.h>
//#include <DX12RHI/DX12Descriptor.h>

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

TEST(DX12RHITest, Mesh)
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

	DX12Instance instance;
	instance.CreateInstance();

	DX12Device device;
	device.CreateDevice(&instance);

	const IBuffer::Description vertexDesc{
		.usage = IBuffer::Usage::USAGE_VERTEX_BUFFER,
		.size = sizeof(Vertex) * vertices.size(),
		.stride = sizeof(Vertex),
		.data = vertices.data()
	};

	DX12Buffer vertexBuffer;
	vertexBuffer.CreateBuffer(&device, vertexDesc);
	vertexBuffer.SetName(&device, "Knight Vertex Buffer");
	vertexBuffer.DestroyBuffer(&device);

	const IBuffer::Description indexDesc{
		.usage = IBuffer::Usage::USAGE_INDEX_BUFFER,
		.size = sizeof(uint16_t) * indices.size(),
		.stride = sizeof(uint16_t),
		.data = indices.data()
	};

	DX12Buffer indexBuffer;
	indexBuffer.CreateBuffer(&device, indexDesc);
	indexBuffer.SetName(&device, "Knight Index Buffer");
	indexBuffer.DestroyBuffer(&device);

	device.DestroyDevice();
	instance.DestroyInstance();
}

TEST(DX12RHITest, Texture)
{
	int width, height, channels;
	unsigned char* data = stbi_load("Assets/Textures/Knight.png", &width, &height, &channels, STBI_rgb_alpha);

	if (!data) return;

	DX12Instance instance;
	instance.CreateInstance();

	DX12Device device;
	device.CreateDevice(&instance);

	DX12Queue graphicsQueue;
	graphicsQueue.CreateQueue(&device, IQueue::Type::QUEUE_TYPE_GRAPHICS);

	DX12CommandAllocator cmdAlloc;
	cmdAlloc.CreateCommandAllocator(&device);

	ICommandBuffer* cmdBuffer = cmdAlloc.AllocateCommandBuffer(&device);

	const IImage::Description imgDesc{
		.width = static_cast<uint32_t>(width),
		.height = static_cast<uint32_t>(height),
		.channelCount = static_cast<uint8_t>(channels),
		.format = IImage::Format::HONOU_FORMAT_R8G8B8A8_UNORM,
		.usage = IImage::Usage((uint64_t)IImage::Usage::HOUNOU_USAGE_COLOR_ATTACHMENT | (uint64_t)IImage::Usage::HOUNOU_USAGE_SAMPLED | (uint64_t)IImage::Usage::HOUNOU_USAGE_TRANSFER_DST)
	};

	DX12Image texture;
	texture.CreateImage(&device, imgDesc);

	const IBuffer::Description stagingDesc{
		.usage = IBuffer::Usage::USAGE_TRANSFER_SRC,
		.size = static_cast<size_t>(width * height * 4),
		.data = data
	};

	DX12Buffer stagingBuffer;
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

TEST(DX12RHITest, GraphicsRenderLoop)
{

}
