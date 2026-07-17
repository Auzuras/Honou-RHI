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

	VkCommandBuffer GetRaw() const { return m_CommandBuffer; }

	void Open() const override final;
	void Close() const override final;

	void Reset() const override final;

	void BeginRendering(IImage* _image, uint32_t _width, uint32_t _height) const override final;
	void EndRendering() const override final;

	void BindPipeline(IPipeline* _pipeline) const override final;

	void SetViewport(float _width, float _height) const override final;
	void SetScissor(uint32_t _width, uint32_t _height) const override final;

	void AddMemoryBarrier0(IImage* _image) const override final;
	void AddMemoryBarrier1(IImage* _image) const override final;
	void Copy(IImage* _destination, IImage* _source) const override final;
	void Copy(IImage* _destination, IBuffer* _source) const override final;

	void BindVertexBuffer(IBuffer* const _vertexBuffer) const override final;
	void BindIndexBuffer(IBuffer* const _indexBuffer) const override final;

	void BindConstants(IPipeline* _pipeline, uint32_t _offset, uint32_t _size, void* _data) const override final;
	void BindDescriptor(IPipeline* _pipeline, IDescriptor* _descriptor, uint32_t _set) const override final;

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
