#ifndef HONOU_RENDER_VKDEVICE_H
#define HONOU_RENDER_VKDEVICE_H

#include <RHI/IDevice.h>

#include <vulkan/vulkan.h>

#include <vector>

class VKDevice : public IDevice
{
public:
	~VKDevice() override final = default;

	void CreateDevice(IInstance* _instance) override final;
	void DestroyDevice() override final;

	VKDevice* const CastToVK() override final { return this; }
	DX12Device* CastToDX12() override final { return nullptr; }
	OGLDevice* CastToOGL() override final { return nullptr; }

	[[nodiscard]] VkDevice const GetLogicalDevice() const { return m_LogicalDevice; }
	[[nodiscard]] VkPhysicalDevice const GetPhysicalDevice() const { return m_PhysicalDevice; }

private:
	uint32_t SelectBestDevice(const std::vector<VkPhysicalDevice>& devices);

private:
	VkDevice m_LogicalDevice = VK_NULL_HANDLE;
	VkPhysicalDevice m_PhysicalDevice = VK_NULL_HANDLE;
	VkQueue m_GraphicsQueue = VK_NULL_HANDLE;
};

#endif //HONOU_RENDER_VKDEVICE_H