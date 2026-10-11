#include <VKRHI/VKSampler.h>

#include <VKRHI/VKDevice.h>

void VKSampler::CreateSampler(IDevice* _device, const Description& _samplerInfo)
{
	const VkDevice ldevice = _device->CastToVK()->GetLogicalDevice();

	const VkSamplerCreateInfo samplerCI{
		.sType				=	VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,

		.magFilter			=	MapSamplerFilterToVkFormat(_samplerInfo.filter),
		.minFilter			=	MapSamplerFilterToVkFormat(_samplerInfo.filter),

		.mipmapMode			=	MapSamplerMipMapModeToVkFormat(_samplerInfo.mipMapMode),

		.addressModeU		=	MapSamplerAddressModeToVkFormat(_samplerInfo.adressModeU),
		.addressModeV		=	MapSamplerAddressModeToVkFormat(_samplerInfo.adressModeV),
		.addressModeW		=	MapSamplerAddressModeToVkFormat(_samplerInfo.adressModeW),

		.anisotropyEnable	=	VK_TRUE,
		.maxAnisotropy		=	8.f,
		.maxLod				=	(float)1
	};

	vkCreateSampler(ldevice, &samplerCI, nullptr, &m_Sampler);
}

void VKSampler::DestroySampler(IDevice* _device)
{
	const VkDevice ldevice = _device->CastToVK()->GetLogicalDevice();
	vkDestroySampler(ldevice, m_Sampler, nullptr);
}
