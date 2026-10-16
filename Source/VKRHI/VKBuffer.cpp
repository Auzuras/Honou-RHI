#include <VKRHI/VKBuffer.h>

#include <VKRHI/VKInstance.h>
#include <VKRHI/VKDevice.h>

void VKBuffer::CreateBuffer(IDevice* _device, const Description& _description)
{
	const VkDevice lDevice = _device->CastToVK()->GetLogicalDevice();
	const VkPhysicalDevice pDevice = _device->CastToVK()->GetPhysicalDevice();
	const VkBufferUsageFlags usage = MapToVulkan(_description.usage);

	VkBufferCreateInfo bufferCI{
		.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
		.size = _description.size,
		.usage = usage,
		.sharingMode = VK_SHARING_MODE_EXCLUSIVE
	};

	vkCreateBuffer(lDevice, &bufferCI, nullptr, &m_Buffer);

	VkMemoryRequirements memRequirements{};
	vkGetBufferMemoryRequirements(lDevice, m_Buffer, &memRequirements);

	VkMemoryAllocateInfo allocInfo{
		.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
		.allocationSize = memRequirements.size,
		.memoryTypeIndex = FindMemoryType(pDevice, memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT),
	};

	vkAllocateMemory(lDevice, &allocInfo, nullptr, &m_BufferMemory);
	vkBindBufferMemory(lDevice, m_Buffer, m_BufferMemory, 0);

	void* data{};
	vkMapMemory(lDevice, m_BufferMemory, 0, bufferCI.size, 0, &data);
	memcpy(data, nullptr, bufferCI.size);
	vkUnmapMemory(lDevice, m_BufferMemory);
}

void VKBuffer::DestroyBuffer(IDevice* _device)
{
	VkDevice lDevice = _device->CastToVK()->GetLogicalDevice();
	vkDestroyBuffer(lDevice, m_Buffer, nullptr);
	vkFreeMemory(lDevice, m_BufferMemory, nullptr);
}

void VKBuffer::SetName(IDevice* _device, const char* _name)
{
	const VkDevice lDevice = _device->CastToVK()->GetLogicalDevice();

	const VkDebugUtilsObjectNameInfoEXT objectNameInfo{
		.sType			=	VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT,
		.objectType		=	VK_OBJECT_TYPE_BUFFER,
		.objectHandle	=	reinterpret_cast<uint64_t>(m_Buffer),
		.pObjectName	=	_name
	};

	Hounou::Internal::vkSetDebugUtilsObjectNameEXT(lDevice, &objectNameInfo);
}

VkBufferUsageFlags VKBuffer::MapToVulkan(Usage _usage)
{
	switch (_usage)
	{
	case IBuffer::Usage::USAGE_VERTEX_BUFFER:		return VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
	case IBuffer::Usage::USAGE_INDEX_BUFFER:		return VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
	case IBuffer::Usage::USAGE_TRANSFER_SRC:		return VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
	case IBuffer::Usage::USAGE_TRANSFER_DST:		return VK_BUFFER_USAGE_TRANSFER_DST_BIT;
	default:										return 0;
	}
}

uint32_t VKBuffer::FindMemoryType(VkPhysicalDevice _physicalDevice, uint32_t _typeFilter, VkMemoryPropertyFlags _properties)
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