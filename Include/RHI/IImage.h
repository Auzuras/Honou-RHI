#ifndef HONOU_RENDER_IIMAGE_H
#define HONOU_RENDER_IIMAGE_H

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
	virtual void DestroyImage() = 0;

	virtual VKImage* CastToVK() { return nullptr; }
	virtual DX12Image* CastToDX12() { return nullptr; }
	virtual OGLImage* CastToOGL() { return nullptr; }
};

#endif //HONOU_RENDER_IIMAGE_H