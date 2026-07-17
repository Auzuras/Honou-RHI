#include <VKRHI/VKDescriptor.h>

#include <VKRHI/VKDevice.h>
#include <VKRHI/VKBuffer.h>
#include <VKRHI/VKImage.h>
#include <VKRHI/VKSampler.h>

void VKDescriptor::Update(IDevice* _device, IBuffer* _buffer) const
{
	const VkDevice lDevice = _device->CastToVK()->GetLogicalDevice();
	const VkBuffer buffer = _buffer->CastToVK()->GetRaw();

	const VkDescriptorBufferInfo bufferInfo{
		.buffer		=	buffer,
		.offset		=	0,
		.range		=	0
	};

	const VkWriteDescriptorSet descriptorWrite{
		.sType				=	VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
		.dstSet				=	m_Descriptor,
		.dstBinding			=	0,
		.dstArrayElement	=	0,
		.descriptorCount	=	1,
		.descriptorType		=	VK_DESCRIPTOR_TYPE_SAMPLER,
		.pBufferInfo		=	&bufferInfo
	};

	vkUpdateDescriptorSets(lDevice, 1, &descriptorWrite, 0, nullptr);
}

void VKDescriptor::Update(IDevice* _device, IImage* _image) const
{
	const VkDevice lDevice = _device->CastToVK()->GetLogicalDevice();
	const VkImageView imageView = _image->CastToVK()->GetRawImageView();

	const VkDescriptorImageInfo imageInfo{
		.imageView		=	imageView,
		.imageLayout	=	VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
	};

	const VkWriteDescriptorSet descriptorWrite{
		.sType				=	VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
		.dstSet				=	m_Descriptor,
		.dstBinding			=	0,
		.dstArrayElement	=	0,
		.descriptorCount	=	1,
		.descriptorType		=	VK_DESCRIPTOR_TYPE_SAMPLER,
		.pImageInfo			=	&imageInfo
	};

	vkUpdateDescriptorSets(lDevice, 1, &descriptorWrite, 0, nullptr);
}

void VKDescriptor::Update(IDevice* _device, ISampler* _sampler) const
{
	const VkDevice lDevice = _device->CastToVK()->GetLogicalDevice();
	const VkSampler sampler = _sampler->CastToVK()->GetRaw();

	const VkDescriptorImageInfo samplerInfo{
		.sampler = sampler
	};

	const VkWriteDescriptorSet descriptorWrite{
		.sType				=	VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
		.dstSet				=	m_Descriptor,
		.dstBinding			=	0,
		.dstArrayElement	=	0,
		.descriptorCount	=	1,
		.descriptorType		=	VK_DESCRIPTOR_TYPE_SAMPLER,
		.pImageInfo			=	&samplerInfo
	};

	vkUpdateDescriptorSets(lDevice, 1, &descriptorWrite, 0, nullptr);
}