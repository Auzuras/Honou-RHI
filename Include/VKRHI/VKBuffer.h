#ifndef HONOU_RENDER_VKBUFFER_H
#define HONOU_RENDER_VKBUFFER_H

class VKBuffer;
class DX12Buffer;
class OGLBuffer;

#include <RHI/IBuffer.h>

#include <vulkan/vulkan.h>

class VKBuffer : public IBuffer
{
public:
	~VKBuffer() override final = default;

	void CreateBuffer(IDevice* _device) override final;
	void DestroyBuffer(IDevice* _device) override final;

	VKBuffer* const CastToVK() override final { return this; }
	DX12Buffer* CastToDX12() override final { return nullptr; }
	OGLBuffer* CastToOGL() override final { return nullptr; }

private:
	VkBuffer m_Buffer = VK_NULL_HANDLE;
};

#endif //HONOU_RENDER_VKBUFFER_H
