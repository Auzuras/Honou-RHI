#include <VKRHI/VKImage.h>

#include <VKRHI/VKDevice.h>

void VKImage::CreateImage(IDevice* _device, const ImageDescription& _description)
{
	VkDevice lDevice = _device->CastToVK()->GetLogicalDevice();

	// Image creation
	{
		m_Image = VKImage::CreateImage(lDevice, _description);
	}

	// Image Memory allocation
	{
		m_ImageMemory = VKImage::AllocateImage(lDevice, m_Image);
	}

	// Image View creation
	{
		m_ImageView = VKImage::CreateImageView(lDevice, m_Image, _description);
	}
}

void VKImage::DestroyImage(IDevice* _device)
{
	VkDevice lDevice = _device->CastToVK()->GetLogicalDevice();

	if (m_ImageView)
		vkDestroyImageView(lDevice, m_ImageView, nullptr);

	//if (m_Image)
	//	vkDestroyImage(lDevice, m_Image, nullptr);

	if (m_ImageMemory)
		vkFreeMemory(lDevice, m_ImageMemory, nullptr);
}

void VKImage::CreateImageInternal(IDevice* _device, VkImage _image, const ImageDescription& _description)
{
	VkDevice lDevice = _device->CastToVK()->GetLogicalDevice();

	m_Image = _image;

	// Image View creation
	{
		m_ImageView = VKImage::CreateImageView(lDevice, m_Image, _description);
	}
}

VkImage VKImage::CreateImage(VkDevice _device, const ImageDescription& _description)
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
		.usage			= VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
		.initialLayout	= VK_IMAGE_LAYOUT_UNDEFINED
	};

	VkImage image = VK_NULL_HANDLE;

	vkCreateImage(_device, &imageCI, nullptr, &image);

	return image;
}

VkDeviceMemory VKImage::AllocateImage(VkDevice _device, VkImage _image)
{
	VkMemoryRequirements memRequirements{};
	vkGetImageMemoryRequirements(_device, _image, &memRequirements);

	const VkMemoryAllocateInfo memoryAI{
		.sType			= VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
		.allocationSize = memRequirements.size
	};

	VkDeviceMemory memory = VK_NULL_HANDLE;

	vkAllocateMemory(_device, &memoryAI, nullptr, &memory);
	vkBindImageMemory(_device, _image, memory, 0);

	return memory;
}

VkImageView VKImage::CreateImageView(VkDevice _device, VkImage _image, const ImageDescription& _description)
{
	const VkImageViewCreateInfo imageViewCI{
		.sType				= VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
		.image				= _image,
		.viewType			= VK_IMAGE_VIEW_TYPE_2D,
		.format				= MapToVkFormat(_description.format),
		.subresourceRange = {
			.aspectMask		= VK_IMAGE_ASPECT_COLOR_BIT,
			.levelCount		= 1,
			.layerCount		= 1
		}
	};

	VkImageView imageView = VK_NULL_HANDLE;

	vkCreateImageView(_device, &imageViewCI, nullptr, &imageView);

	return imageView;
}