#ifndef HONOU_RENDER_VKCOMMANDBUFFER_H
#define HONOU_RENDER_VKCOMMANDBUFFER_H

class VKCommandBuffer;
class DX12CommandBuffer;
class OGLCommandBuffer;

class VKCommandAllocator;

#include <RHI/ICommandBuffer.h>

#include <vulkan/vulkan.h>

class VKCommandBuffer : public ICommandBuffer
{
public:
	~VKCommandBuffer() override final = default;

	void Open() const override final;
	void Close() const override final;

	VKCommandBuffer* const CastToVK() { return this; }
	DX12CommandBuffer* CastToDX12() { return nullptr; }
	OGLCommandBuffer* CastToOGL() { return nullptr; }

private:
	friend class VKCommandAllocator;

	VkCommandBuffer m_CommandBuffer = VK_NULL_HANDLE;
};

#endif //HONOU_RENDER_VKCOMMANDBUFFER_H
