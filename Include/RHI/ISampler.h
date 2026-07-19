#ifndef HONOU_RHI_ISAMPLER_H
#define HONOU_RHI_ISAMPLER_H

#include <cstdint>

class VKSampler;
class DX12Sampler;
class OGLSampler;

class IDevice;

class ISampler
{
public:

	// Base sampler info struct (WIP)
	struct SamplerInfo
	{
		enum class Filter : uint8_t
		{
			FILTER_LINEAR = 0,
			FILTER_NEAREST = 1
		};

		enum class AddressMode : uint8_t
		{
			REPEAT = 0,
		};

		enum class MipMapMode : uint8_t
		{
			MIPMAP_MODE_NEAREST = 0,
			MIPMAP_MODE_LINEAR = 1
		};

		AddressMode adressModeU = AddressMode::REPEAT;
		AddressMode adressModeV = AddressMode::REPEAT;
		AddressMode adressModeW = AddressMode::REPEAT;

		Filter filter = Filter::FILTER_NEAREST;

		MipMapMode mipMapMode = MipMapMode::MIPMAP_MODE_NEAREST;
	};

	virtual ~ISampler() = default;

	virtual void CreateSampler(IDevice* _device, const SamplerInfo& _samplerInfo) = 0;
	virtual void DestroySampler(IDevice* _device) = 0;

	virtual VKSampler* CastToVK() { return nullptr; }
	virtual DX12Sampler* CastToDX12() { return nullptr; }
	virtual OGLSampler* CastToOGL() { return nullptr; }
};

#endif //HONOU_RHI_ISAMPLER_H
