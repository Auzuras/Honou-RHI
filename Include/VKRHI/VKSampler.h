#ifndef HONOU_RHI_VKSAMPLER_H
#define HONOU_RHI_VKSAMPLER_H

#include <RHI/ISampler.h>

#include <vulkan/vulkan.h>

class VKSampler : public ISampler
{
public:
	~VKSampler() override final = default;

	virtual void CreateSampler(IDevice* _device, const Description& _samplerInfo) override final;
	virtual void DestroySampler(IDevice* _device) override final;

	virtual VKSampler* CastToVK() override final { return this; }
	virtual DX12Sampler* CastToDX12() override final { return nullptr; }
	virtual OGLSampler* CastToOGL() override final { return nullptr; }

	VkSampler GetRaw() const { return m_Sampler; }

private:

	static VkFilter MapSamplerFilterToVkFormat(const Description::Filter _filter)
	{
		switch (_filter)
		{
		case Description::Filter::FILTER_LINEAR: default: return VK_FILTER_LINEAR;
		case Description::Filter::FILTER_NEAREST: return VK_FILTER_NEAREST;
		}
	}

	static VkSamplerAddressMode MapSamplerAddressModeToVkFormat(const Description::AddressMode _adressMode)
	{
		switch (_adressMode)
		{
		case Description::AddressMode::REPEAT: default: return VK_SAMPLER_ADDRESS_MODE_REPEAT;
		}
	}

	static VkSamplerMipmapMode MapSamplerMipMapModeToVkFormat(const Description::MipMapMode _mipMapMode)
	{
		switch (_mipMapMode)
		{
		case Description::MipMapMode::MIPMAP_MODE_NEAREST: default: return VK_SAMPLER_MIPMAP_MODE_NEAREST;
		case Description::MipMapMode::MIPMAP_MODE_LINEAR: return VK_SAMPLER_MIPMAP_MODE_LINEAR;
		}
	}

	VkSampler m_Sampler = VK_NULL_HANDLE;
};

#endif //HONOU_RHI_VKSAMPLER_H