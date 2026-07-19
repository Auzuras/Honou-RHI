#ifndef HONOU_RHI_VKIMAGE_H
#define HONOU_RHI_VKIMAGE_H

#include <RHI/IImage.h>

#include <vulkan/vulkan.h>

class VKImage : public IImage
{
public:
	~VKImage() override final = default;

	virtual void CreateImage(IDevice* _device, const ImageDescription& _description) override final;
	virtual void DestroyImage(IDevice* _device) override final;

	void CreateImageInternal(IDevice* _device, VkImage _image, const ImageDescription& _description);


	virtual VKImage* CastToVK() override final { return this; }
	virtual DX12Image* CastToDX12() override final { return nullptr; }
	virtual OGLImage* CastToOGL() override final { return nullptr; }

	VkImage GetRaw() const { return m_Image; }
	VkImageView GetRawImageView() const { return m_ImageView; }

protected:

	static VkImage CreateImage(VkDevice _device, const ImageDescription& _description);
	static VkDeviceMemory AllocateImage(VkDevice _device, VkPhysicalDevice _physicalDevice, VkImage _image);
	static VkImageView CreateImageView(VkDevice _device, VkImage _image, const ImageDescription& _description);

	void UploadData(IDevice* _device, const ImageDescription& _description);

private:

	VkImage m_Image = VK_NULL_HANDLE;
	VkDeviceMemory m_ImageMemory = VK_NULL_HANDLE;
	VkImageView m_ImageView = VK_NULL_HANDLE;
};

static inline VkFormat MapToVkFormat(const TextureFormat _format)
{
	// TEMP WILL MOVE IN AN OTHER FILE
	switch (_format)
	{
	default:
	case TextureFormat::HONOU_FORMAT_UNDEFINED:				return VK_FORMAT_UNDEFINED;
	case TextureFormat::HONOU_FORMAT_R8G8B8A8_UNORM:		return VK_FORMAT_R8G8B8A8_UNORM;
	case TextureFormat::HONOU_FORMAT_B8G8R8A8_UNORM:		return VK_FORMAT_B8G8R8A8_UNORM;
	case TextureFormat::HONOU_FORMAT_R8G8B8_UNORM:			return VK_FORMAT_R8G8B8_UNORM;
	case TextureFormat::HONOU_FORMAT_B8G8R8_UNORM:			return VK_FORMAT_B8G8R8_UNORM;
	case TextureFormat::HONOU_FORMAT_R8G8B8A8_SRGB:			return VK_FORMAT_R8G8B8A8_SRGB;
	case TextureFormat::HONOU_FORMAT_B8G8R8A8_SRGB:			return VK_FORMAT_B8G8R8A8_SRGB;
	case TextureFormat::HONOU_FORMAT_R8G8B8_SRGB:			return VK_FORMAT_R8G8B8_SRGB;
	case TextureFormat::HONOU_FORMAT_B8G8R8_SRGB:			return VK_FORMAT_B8G8R8_SRGB;
	case TextureFormat::HONOU_FORMAT_R16G16B16A16_FLOAT:	return VK_FORMAT_R16G16B16A16_SFLOAT;
	case TextureFormat::HONOU_FORMAT_R32G32B32A32_FLOAT:	return VK_FORMAT_R32G32B32A32_SFLOAT;
	case TextureFormat::HOUNOU_FORMAT_D32_FLOAT:			return VK_FORMAT_D32_SFLOAT;
	case TextureFormat::HOUNOU_FORMAT_D32_FLOAT_S8_UINT:	return VK_FORMAT_D32_SFLOAT_S8_UINT;
	case TextureFormat::HOUNOU_FORMAT_D24_UNORM_S8_UINT:	return VK_FORMAT_D24_UNORM_S8_UINT;
	}
}

#endif //HONOU_RHI_VKIMAGE_H