#ifndef HONOU_RENDER_ICOMMANDBUFFER_H
#define HONOU_RENDER_ICOMMANDBUFFER_H

#include <cstdint>

class VKCommandBuffer;
class DX12CommandBuffer;
class OGLCommandBuffer;

class IDevice;
class IBuffer;

class ICommandBuffer
{
public:
	virtual ~ICommandBuffer() = default;

	virtual void Open() const;
	virtual void Close() const;

	virtual void SetViewport(float _width, float _height) const;
	virtual void SetScissor(uint32_t _width, uint32_t _height) const;

	virtual void BindVertexBuffer(IBuffer* const _vertexBuffer) const;
	virtual void BindIndexBuffer(IBuffer* const _indexBuffer) const;

	virtual void Draw(uint32_t _vertexCount) const;
	virtual void DrawIndexed(uint32_t _indexCount) const;
	virtual void Dispatch(uint32_t _groupCountX, uint32_t _groupCountY, uint32_t _groupCountZ) const;

	virtual VKCommandBuffer* const CastToVK() { return nullptr; }
	virtual DX12CommandBuffer* CastToDX12() { return nullptr; }
	virtual OGLCommandBuffer* CastToOGL() { return nullptr; }
};

#endif //HONOU_RENDER_ICOMMANDBUFFER_H
