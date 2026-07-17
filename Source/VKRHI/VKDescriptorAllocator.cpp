#include <VKRHI/VKDescriptorAllocator.h>

#include <VKRHI/VKDevice.h>
#include <VKRHI/VKDescriptor.h>

#include <array>

void VKDescriptorAllocator::CreateDescriptorAllocator(IDevice* _device)
{
	const VkDevice lDevice = _device->CastToVK()->GetLogicalDevice();
	const uint32_t maxSet = 10;

	std::array<VkDescriptorPoolSize, 3> pools{
		VkDescriptorPoolSize{
			.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
			.descriptorCount = maxSet
		},
		VkDescriptorPoolSize{
			.type = VK_DESCRIPTOR_TYPE_SAMPLER,
			.descriptorCount = maxSet
		},
		VkDescriptorPoolSize{
			.type = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE,
			.descriptorCount = maxSet
		}
	};

	const VkDescriptorPoolCreateInfo poolCI{
		.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
		.maxSets = maxSet,
		.poolSizeCount = pools.size(),
		.pPoolSizes = pools.data()
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
	const VkDevice lDevice = _device->CastToVK()->GetLogicalDevice();

	const VkDescriptorSetAllocateInfo allocateInfo{
		.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
		.descriptorPool = m_DescriptorAllocator,
		.descriptorSetCount = 1,
		.pSetLayouts = VK_NULL_HANDLE
	};

	vkAllocateDescriptorSets(lDevice, &allocateInfo, VK_NULL_HANDLE);

	return nullptr;
}

IDescriptor* VKDescriptorAllocator::AllocateDescriptor(IDevice* _device)
{
	return nullptr;
}

void VKDescriptorAllocator::FreeDescriptors(IDevice* _device, IDescriptor* _descriptor, uint32_t _count)
{

}

void VKDescriptorAllocator::FreeDescriptor(IDevice* _device, IDescriptor* _descriptor)
{
	const VkDevice lDevice = _device->CastToVK()->GetLogicalDevice();
	const VkDescriptorSet descriptor = _descriptor->CastToVK()->GetRaw();
	vkFreeDescriptorSets(lDevice, m_DescriptorAllocator, 1, &descriptor);
}