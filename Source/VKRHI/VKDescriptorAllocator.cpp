#include <VKRHI/VKDescriptorAllocator.h>

#include <VKRHI/VKDevice.h>
#include <VKRHI/VKPipeline.h>
#include <VKRHI/VKDescriptor.h>

#include <array>

void VKDescriptorAllocator::CreateDescriptorAllocator(IDevice* _device)
{
	const VkDevice lDevice = _device->CastToVK()->GetLogicalDevice();
	const uint32_t maxSet = 10;

	std::array<VkDescriptorPoolSize, 4> pools{
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
		},
		VkDescriptorPoolSize{
			.type = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,
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
	vkResetDescriptorPool(lDevice, m_DescriptorAllocator, 0);
	vkDestroyDescriptorPool(lDevice, m_DescriptorAllocator, nullptr);
}

std::vector<IDescriptor*> VKDescriptorAllocator::AllocateDescriptors(IDevice* _device, IPipeline* _pipeline, uint32_t _count)
{
	const VkDevice lDevice = _device->CastToVK()->GetLogicalDevice();
	const VkDescriptorSetLayout layout = _pipeline->CastToVK()->GetDescriptorLayout();

	std::vector<VkDescriptorSetLayout> layouts(_count, layout);

	const VkDescriptorSetAllocateInfo allocateInfo{
		.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
		.descriptorPool = m_DescriptorAllocator,
		.descriptorSetCount = _count,
		.pSetLayouts = layouts.data()
	};

	std::vector<VkDescriptorSet> vkDescriptors{ _count };
	vkAllocateDescriptorSets(lDevice, &allocateInfo, vkDescriptors.data());

	std::vector<IDescriptor*> descriptors = std::vector<IDescriptor*>(_count);
	for (size_t i = 0; i < _count; ++i)
	{
		VKDescriptor* descriptor = new VKDescriptor();
		descriptor->m_Descriptor = vkDescriptors[i];
		descriptors[i] = descriptor;
	}

	return descriptors;
}

IDescriptor* VKDescriptorAllocator::AllocateDescriptor(IDevice* _device, IPipeline* _pipeline)
{
	return AllocateDescriptors(_device, _pipeline, 1)[0];
}

void VKDescriptorAllocator::FreeDescriptors(IDevice* _device, const std::vector<IDescriptor*>& _descriptors)
{
	// Require a flag or to reset the pool
	for (size_t i = 0; i < _descriptors.size(); ++i)
		FreeDescriptor(_device, _descriptors[i]);
}

void VKDescriptorAllocator::FreeDescriptor(IDevice* _device, IDescriptor* _descriptor)
{
	const VkDevice lDevice = _device->CastToVK()->GetLogicalDevice();
	const VkDescriptorSet descriptor = _descriptor->CastToVK()->GetRaw();
	vkFreeDescriptorSets(lDevice, m_DescriptorAllocator, 1, &descriptor);
}