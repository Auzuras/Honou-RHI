#include <VKRHI/VKDescriptorAllocator.h>

#include <VKRHI/VKDevice.h>
#include <VKRHI/VKDescriptor.h>

void VKDescriptorAllocator::CreateDescriptorAllocator(IDevice* _device)
{
	const VkDevice lDevice = _device->CastToVK()->GetLogicalDevice();
	const uint32_t maxSet = 2;

	VkDescriptorPoolSize poolSize{
		.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
		.descriptorCount = maxSet
	};

	VkDescriptorPoolCreateInfo poolCI{
		.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
		.maxSets = maxSet,
		.poolSizeCount = 1,
		.pPoolSizes = &poolSize
	};

	vkCreateDescriptorPool(lDevice, &poolCI, nullptr, &m_DescriptorAllocator);
}

void VKDescriptorAllocator::DestroyDescriptorAllocator(IDevice* _device)
{
	const VkDevice lDevice = _device->CastToVK()->GetLogicalDevice();
	vkDestroyDescriptorPool(lDevice, m_DescriptorAllocator, nullptr);
}

IDescriptor* VKDescriptorAllocator::AllocateDescriptors(IDevice* _device, uint32_t _count)
{

}

IDescriptor* VKDescriptorAllocator::AllocateDescriptor(IDevice* _device)
{

}

void VKDescriptorAllocator::FreeDescriptors(IDevice* _device, IDescriptor* _descriptor, uint32_t _count)
{

}

void VKDescriptorAllocator::FreeDescriptor(IDevice* _device, IDescriptor* _descriptor)
{

}