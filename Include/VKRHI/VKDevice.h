#ifndef HONOU_RENDER_VKDEVICE_H
#define HONOU_RENDER_VKDEVICE_H

#include <RHI/IDevice.h>

#include <vulkan/vulkan.h>

#include <vector>

class VKDevice : public IDevice
{
public:
	~VKDevice() override final { };

	void CreateDevice(IInstance* _instance) override final;
	void DestroyDevice() override final;

	VKDevice* CastToVK() override final { return this; }
	DX12Device* CastToDX12() override final { return nullptr; }
	OGLDevice* CastToOGL() override final { return nullptr; }

private:

	uint32_t SelectBestDevice(const std::vector<VkPhysicalDevice>& devices);

private:

	VkDevice m_Device = VK_NULL_HANDLE;
	VkQueue m_GraphicsQueue = VK_NULL_HANDLE;
};

#endif //HONOU_RENDER_VKDEVICE_H