#ifndef HONOU_RHI_IIMAGE_H
#define HONOU_RHI_IIMAGE_H

class VKImage;
class DX12Image;
class OGLImage;

class IDevice;

#include <Helpers/Image.h>

class IImage
{
public:
	virtual ~IImage() = default;

	virtual void CreateImage(IDevice* _device, const ImageDescription& _description) = 0;
	virtual void DestroyImage(IDevice* _device) = 0;

	virtual uint32_t GetWidth() const { return p_Width; }
	virtual uint32_t GetHeight() const { return p_Height; }
	virtual uint8_t GetChannelCount() const { return p_ChannelCount; }
	virtual TextureFormat GetFormat() const { return p_Format; }

	virtual VKImage* CastToVK() { return nullptr; }
	virtual DX12Image* CastToDX12() { return nullptr; }
	virtual OGLImage* CastToOGL() { return nullptr; }

protected:

	uint32_t p_Width = 0u;
	uint32_t p_Height = 0u;
	uint8_t p_ChannelCount = 0;
	TextureFormat p_Format = TextureFormat::HONOU_FORMAT_UNDEFINED;

	bool p_FromSwapChain = false;
};

#endif //HONOU_RHI_IIMAGE_H