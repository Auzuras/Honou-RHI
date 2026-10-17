#ifndef HONOU_RHI_DX12IMAGE_H
#define HONOU_RHI_DX12IMAGE_H

#include <RHI/IImage.h>

#include <wrl.h>
#include <d3d12.h>

class DX12Image : public IImage
{
public:
	~DX12Image() override final = default;

	void CreateImage(IDevice* _device, const Description& _description) override final;
	void DestroyImage(IDevice* _device) override final;

	void SetName(IDevice* _device, const char* _name) override final;

	void CreateSwapChainImage(IDevice* _device, ID3D12Resource* _image, const Description& _description);
	void DestroySwapChainImage(IDevice* _device);

	virtual VKImage* CastToVK() override final { return nullptr; }
	virtual DX12Image* CastToDX12() override final { return this; }
	virtual OGLImage* CastToOGL() override final { return nullptr; }

private:

	Microsoft::WRL::ComPtr<ID3D12Resource> m_Image = nullptr;
};

static inline DXGI_FORMAT MapToDX12Format(const IImage::Format _format)
{
	// TEMP WILL MOVE IN AN OTHER FILE
	switch (_format)
	{
	default:
	case IImage::Format::HONOU_FORMAT_UNDEFINED:			return DXGI_FORMAT_UNKNOWN;
	case IImage::Format::HONOU_FORMAT_R8G8B8A8_UNORM:		return DXGI_FORMAT_R8G8B8A8_UNORM;
	case IImage::Format::HONOU_FORMAT_B8G8R8A8_UNORM:		return DXGI_FORMAT_B8G8R8A8_UNORM;
	case IImage::Format::HONOU_FORMAT_R8G8B8A8_SRGB:		return DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	case IImage::Format::HONOU_FORMAT_B8G8R8A8_SRGB:		return DXGI_FORMAT_B8G8R8A8_UNORM_SRGB;
	case IImage::Format::HONOU_FORMAT_R16G16B16A16_FLOAT:	return DXGI_FORMAT_R16G16B16A16_FLOAT;
	case IImage::Format::HONOU_FORMAT_R32G32B32A32_FLOAT:	return DXGI_FORMAT_R32G32B32A32_FLOAT;
	case IImage::Format::HOUNOU_FORMAT_D32_FLOAT:			return DXGI_FORMAT_D32_FLOAT;
	case IImage::Format::HOUNOU_FORMAT_D32_FLOAT_S8_UINT:	return DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
	case IImage::Format::HOUNOU_FORMAT_D24_UNORM_S8_UINT:	return DXGI_FORMAT_D24_UNORM_S8_UINT;
	}
}

#endif //HONOU_RHI_DX12IMAGE_H