#include <VKRHI/VKImage.h>

#include <VKRHI/VKInstance.h>
#include <VKRHI/VKDevice.h>
#include <VKRHI/VKBuffer.h>
#include <VKRHI/VKCommandAllocator.h>
#include <VKRHI/VKCommandBuffer.h>
#include <VKRHI/VKQueue.h>

static VkImageUsageFlags ToVkImageUsage(uint64_t _usage)
{
	VkImageUsageFlags flags = 0;

	if ((_usage & (uint64_t)IImage::Usage::HOUNOU_USAGE_COLOR_ATTACHMENT) != 0)
		flags |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	if ((_usage & (uint64_t)IImage::Usage::HOUNOU_USAGE_DEPTH_STENCIL_ATTACHMENT) != 0)
		flags |= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
	if ((_usage & (uint64_t)IImage::Usage::HOUNOU_USAGE_TRANSFER_SRC) != 0)
		flags |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
	if ((_usage & (uint64_t)IImage::Usage::HOUNOU_USAGE_TRANSFER_DST) != 0)
		flags |= VK_IMAGE_USAGE_TRANSFER_DST_BIT;
	if ((_usage & (uint64_t)IImage::Usage::HOUNOU_USAGE_SAMPLED) != 0)
		flags |= VK_IMAGE_USAGE_SAMPLED_BIT;
	if ((_usage & (uint64_t)IImage::Usage::HOUNOU_USAGE_STORAGE) != 0)
		flags |= VK_IMAGE_USAGE_STORAGE_BIT;

	return flags;
}

void VKImage::CreateImage(IDevice* _device, const Description& _description)
{
	p_Width = _description.width; p_Height = _description.height; p_ChannelCount = _description.channelCount;
	p_Format = _description.format;

	VkDevice lDevice = _device->CastToVK()->GetLogicalDevice();
	VkPhysicalDevice pDevice = _device->CastToVK()->GetPhysicalDevice();

	// Image creation
	{
		m_Image = VKImage::CreateImage(lDevice, _description);
	}

	// Image Memory allocation
	{
		m_ImageMemory = VKImage::AllocateImage(lDevice, pDevice, m_Image);
	}

	// Image View creation
	{
		m_ImageView = VKImage::CreateImageView(lDevice, m_Image, _description);
	}

	// Data Upload
	if (_description.data)
	{
		UploadData(_device, _description);
	}
}

void VKImage::DestroyImage(IDevice* _device)
{
	VkDevice lDevice = _device->CastToVK()->GetLogicalDevice();

	if (m_ImageView)
		vkDestroyImageView(lDevice, m_ImageView, nullptr);

	if (!p_FromSwapChain && m_Image)
		vkDestroyImage(lDevice, m_Image, nullptr);

	if (m_ImageMemory)
		vkFreeMemory(lDevice, m_ImageMemory, nullptr);
}

void VKImage::SetName(IDevice* _device, const char* _name)
{
	const VkDevice lDevice = _device->CastToVK()->GetLogicalDevice();

	const VkDebugUtilsObjectNameInfoEXT objectNameInfo{
		.sType			=	VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT,
		.objectType		=	VK_OBJECT_TYPE_IMAGE,
		.objectHandle	=	reinterpret_cast<uint64_t>(m_Image),
		.pObjectName	=	_name
	};

	Hounou::Internal::vkSetDebugUtilsObjectNameEXT(lDevice, &objectNameInfo);
}


void VKImage::CreateImageInternal(IDevice* _device, VkImage _image, const Description& _description)
{
	p_Width = _description.width; p_Height = _description.height; p_ChannelCount = _description.channelCount;
	p_Format = _description.format;

	VkDevice lDevice = _device->CastToVK()->GetLogicalDevice();

	p_FromSwapChain = true;

	m_Image = _image;

	// Image View creation
	{
		m_ImageView = VKImage::CreateImageView(lDevice, m_Image, _description);
	}
}

VkImage VKImage::CreateImage(VkDevice _device, const Description& _description)
{
	const VkImageCreateInfo imageCI{
		.sType			= VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
		.imageType		= VK_IMAGE_TYPE_2D,
		.format			= MapToVkFormat(_description.format),
		.extent = {
			.width		= _description.width,
			.height		= _description.height,
			.depth		= 1
		},
		.mipLevels		= 1,
		.arrayLayers	= 1,
		.samples		= VK_SAMPLE_COUNT_1_BIT,
		.tiling			= VK_IMAGE_TILING_OPTIMAL,
		.usage			= static_cast<VkImageUsageFlags>(ToVkImageUsage(static_cast<uint64_t>(_description.usage))),
		.initialLayout	= VK_IMAGE_LAYOUT_UNDEFINED
	};

	VkImage image = VK_NULL_HANDLE;

	vkCreateImage(_device, &imageCI, nullptr, &image);

	return image;
}

uint32_t FindMemoryType(VkPhysicalDevice _physicalDevice, uint32_t _typeFilter, VkMemoryPropertyFlags _properties)
{
	VkPhysicalDeviceMemoryProperties memProperties{};
	vkGetPhysicalDeviceMemoryProperties(_physicalDevice, &memProperties);

	for (uint32_t i = 0; i < memProperties.memoryTypeCount; ++i)
	{
		if ((_typeFilter & (1 << i))
			&& ((memProperties.memoryTypes[i].propertyFlags & _properties) == _properties))
			return i;
	}

	return 0;
}

VkDeviceMemory VKImage::AllocateImage(VkDevice _device, VkPhysicalDevice _physicalDevice, VkImage _image)
{
	VkMemoryRequirements memRequirements{};
	vkGetImageMemoryRequirements(_device, _image, &memRequirements);

	const VkMemoryAllocateInfo memoryAI{
		.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
		.allocationSize = memRequirements.size,
		.memoryTypeIndex = FindMemoryType(_physicalDevice, memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)
	};

	VkDeviceMemory memory = VK_NULL_HANDLE;

	vkAllocateMemory(_device, &memoryAI, nullptr, &memory);
	vkBindImageMemory(_device, _image, memory, 0);

	return memory;
}

VkImageView VKImage::CreateImageView(VkDevice _device, VkImage _image, const Description& _description)
{
	const VkImageViewCreateInfo imageViewCI{
		.sType				= VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
		.image				= _image,
		.viewType			= VK_IMAGE_VIEW_TYPE_2D,
		.format				= MapToVkFormat(_description.format),
		.subresourceRange = {
			.aspectMask		= _description.format == Format::HOUNOU_FORMAT_D32_FLOAT_S8_UINT
		? static_cast<VkImageAspectFlags>(VK_IMAGE_ASPECT_DEPTH_BIT)
		: static_cast<VkImageAspectFlags>(VK_IMAGE_ASPECT_COLOR_BIT),
			.levelCount		= 1,
			.layerCount		= 1
		}
	};

	VkImageView imageView = VK_NULL_HANDLE;

	vkCreateImageView(_device, &imageViewCI, nullptr, &imageView);

	return imageView;
}

void VKImage::UploadData(IDevice* _device, const Description& _description)
{
	VkDeviceSize totalSize = _description.width * _description.height * 4;

	const IBuffer::Description stagingDesc{
		.usage	=	IBuffer::Usage::USAGE_TRANSFER_SRC,
		.size	=	totalSize,
		.data	=	_description.data
	};

	VKBuffer stagingBuffer;
	stagingBuffer.CreateBuffer(_device, stagingDesc);

	VKCommandAllocator allocator;
	allocator.CreateCommandAllocator(_device);
	ICommandBuffer* cmdBuffer = allocator.AllocateCommandBuffer(_device);

	cmdBuffer->Open();

	const ICommandBuffer::MemoryBarrierInfo info{
		.oldLayout = ICommandBuffer::Layout::UNDEFINED,
		.newLayout = ICommandBuffer::Layout::TRANSFER_DST,

		.syncDest{
			.stages = ICommandBuffer::PipelineStage::TRANSFER,
			.access = ICommandBuffer::Access::TRANSFER_WRITE
		}
	};

	cmdBuffer->AddMemoryBarrier(this, info);

	cmdBuffer->Copy(this, &stagingBuffer);

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

	cmdBuffer->AddMemoryBarrier(this, info2);

	cmdBuffer->Close();

	_device->GetGraphicsQueue()->SubmitCommandBuffer(cmdBuffer);

	_device->GetGraphicsQueue()->WaitIdle();

	stagingBuffer.DestroyBuffer(_device);

	allocator.FreeCommandBuffer(_device, cmdBuffer);

	allocator.DestroyCommandAllocator(_device);
}