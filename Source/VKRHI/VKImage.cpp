#include <VKRHI/VKImage.h>

#include <VKRHI/VKDevice.h>

void VKImage::CreateImage(IDevice* _device, const ImageDescription& _description)
{
	VkDevice lDevice = _device->CastToVK()->GetLogicalDevice();

	// Image creation
	{
		VKImage::CreateImage(lDevice, &m_Image, _description);
	}

	// Image Memory allocation
	{
		VKImage::AllocateImage(lDevice, &m_Image, &m_ImageMemory);
	}

	// Image View creation
	{
		VKImage::CreateImageView(lDevice, &m_Image, &m_ImageView, _description);
	}
}

VkImage VKImage::CreateImage(VkDevice _device, VkImage* _image, const ImageDescription& _description)
{
	VkImageCreateInfo imageCI{
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

	vkCreateImage(_device, &imageCI, nullptr, _image);
}

VkDeviceMemory VKImage::AllocateImage(VkDevice _device, VkImage* _image, VkDeviceMemory* _memory)
{
	VkMemoryRequirements memRequirements{};
	vkGetImageMemoryRequirements(_device, *_image, &memRequirements);

	VkMemoryAllocateInfo memoryAI{
		.sType			= VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
		.allocationSize = memRequirements.size
	};

	vkAllocateMemory(_device, &memoryAI, nullptr, _memory);
	vkBindImageMemory(_device, *_image, *_memory, 0);
}

VkDeviceMemory CreateImageView(VkDevice _device, VkImage* _image, VkImageView* _imageView, const ImageDescription& _description)
{
	VkImageViewCreateInfo imageViewCI{
		.sType				= VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
		.image				= *_image,
		.viewType			= VK_IMAGE_VIEW_TYPE_2D,
		.format				= MapToVkFormat(_description.format),
		.subresourceRange = {
			.aspectMask		= VK_IMAGE_ASPECT_COLOR_BIT,
			.levelCount		= 1,
			.layerCount		= 1
		}
	};

	vkCreateImageView(_device, &imageViewCI, nullptr, _imageView);
}