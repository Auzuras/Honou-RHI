#ifndef HONOU_RHI_IIMAGE_H
#define HONOU_RHI_IIMAGE_H

#include <cstdint>

class VKImage;
class DX12Image;
class OGLImage;

class IDevice;

class IImage
{
public:

	enum class Format : uint32_t
	{
		HONOU_FORMAT_UNDEFINED = 0,

		HONOU_FORMAT_R8G8B8A8_UNORM = 1,
		HONOU_FORMAT_B8G8R8A8_UNORM = 2,

		HONOU_FORMAT_R8G8B8A8_SRGB = 3,
		HONOU_FORMAT_B8G8R8A8_SRGB = 4,

		HONOU_FORMAT_R16G16B16A16_FLOAT = 5,
		HONOU_FORMAT_R32G32B32A32_FLOAT = 6,

		HOUNOU_FORMAT_D32_FLOAT = 7,
		HOUNOU_FORMAT_D32_FLOAT_S8_UINT = 8,
		HOUNOU_FORMAT_D24_UNORM_S8_UINT = 9
	};

	enum class Usage : uint64_t
	{
		HOUNOU_USAGE_NONE = 0,

		HOUNOU_USAGE_COLOR_ATTACHMENT = 1 << 0ull,
		HOUNOU_USAGE_DEPTH_STENCIL_ATTACHMENT = 1 << 1ull,

		HOUNOU_USAGE_TRANSFER_SRC = 1 << 2ull,
		HOUNOU_USAGE_TRANSFER_DST = 1 << 3ull,

		HOUNOU_USAGE_SAMPLED = 1 << 4ull,

		HOUNOU_USAGE_STORAGE = 1 << 5ull,
	};

	struct Description
	{
		uint32_t width			=	0u;
		uint32_t height			=	0u;
		uint8_t channelCount	=	0u;
		Format format			=	Format::HONOU_FORMAT_R8G8B8A8_UNORM;
		Usage usage				=	Usage::HOUNOU_USAGE_NONE;
	};

	virtual ~IImage() = default;

	virtual void CreateImage(IDevice* _device, const Description& _description) = 0;
	virtual void DestroyImage(IDevice* _device) = 0;

	virtual void SetName(IDevice* _device, const char* _name) = 0;

	virtual uint32_t GetWidth() const { return p_Width; }
	virtual uint32_t GetHeight() const { return p_Height; }
	virtual uint8_t GetChannelCount() const { return p_ChannelCount; }
	virtual Format GetFormat() const { return p_Format; }

	virtual VKImage* CastToVK() { return nullptr; }
	virtual DX12Image* CastToDX12() { return nullptr; }
	virtual OGLImage* CastToOGL() { return nullptr; }

protected:

	uint32_t p_Width = 0u;
	uint32_t p_Height = 0u;
	uint8_t p_ChannelCount = 0u;
	Format p_Format = Format::HONOU_FORMAT_UNDEFINED;

	bool p_FromSwapChain = false;
};

#endif //HONOU_RHI_IIMAGE_H