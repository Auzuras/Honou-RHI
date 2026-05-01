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

	void SetViewport(float _width, float _height) const override final;
	void SetScissor(uint32_t _width, uint32_t _height) const override final;

	void BindVertexBuffer(IBuffer* const _vertexBuffer) const override final;
	void BindIndexBuffer(IBuffer* const _indexBuffer) const override final;

	void Draw(uint32_t _vertexCount) const override final;
	void DrawIndexed(uint32_t _indexCount) const override final;
	void Dispatch(uint32_t _groupCountX, uint32_t _groupCountY, uint32_t _groupCountZ) const override final;

	VKCommandBuffer* const CastToVK() { return this; }
	DX12CommandBuffer* CastToDX12() { return nullptr; }
	OGLCommandBuffer* CastToOGL() { return nullptr; }

private:
	friend class VKCommandAllocator;

	VkCommandBuffer m_CommandBuffer = VK_NULL_HANDLE;
};

#endif //HONOU_RENDER_VKCOMMANDBUFFER_H
