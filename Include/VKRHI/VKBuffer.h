#ifndef HONOU_RHI_VKBUFFER_H
#define HONOU_RHI_VKBUFFER_H

class VKBuffer;
class DX12Buffer;
class OGLBuffer;

#include <RHI/IBuffer.h>

#include <vulkan/vulkan.h>

class VKBuffer : public IBuffer
{
public:
	~VKBuffer() override final = default;

	void CreateBuffer(IDevice* _device, void* _data, size_t _size, BufferUsage _usage) override final;
	void DestroyBuffer(IDevice* _device) override final;

	void SetName(IDevice* _device, const char* _name) override final;

	VKBuffer* const CastToVK() override final { return this; }
	DX12Buffer* CastToDX12() override final { return nullptr; }
	OGLBuffer* CastToOGL() override final { return nullptr; }

	VkBuffer GetRaw() { return m_Buffer; }

private:

	static VkBufferUsageFlags MapToVulkan(BufferUsage _usage);
	static uint32_t FindMemoryType(VkPhysicalDevice _physicalDevice, uint32_t _typeFilter, VkMemoryPropertyFlags _properties);

private:

	VkBuffer m_Buffer = VK_NULL_HANDLE;
	VkDeviceMemory m_BufferMemory = VK_NULL_HANDLE;
};

#endif //HONOU_RHI_VKBUFFER_H
