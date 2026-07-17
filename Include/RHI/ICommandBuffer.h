#ifndef HONOU_RENDER_ICOMMANDBUFFER_H
#define HONOU_RENDER_ICOMMANDBUFFER_H

#include <cstdint>

class VKCommandBuffer;
class DX12CommandBuffer;
class OGLCommandBuffer;

class IDevice;
class IBuffer;
class IImage;
class IPipeline;

class ICommandBuffer
{
public:
	virtual ~ICommandBuffer() = default;

	virtual void Open() const = 0;
	virtual void Close() const = 0;

	virtual void Reset() const = 0;

	virtual void BeginRendering(IImage* _image, uint32_t _width, uint32_t _height) const = 0;
	virtual void EndRendering() const = 0;

	virtual void BindPipeline(IPipeline* _pipeline) const = 0;

	virtual void SetViewport(float _width, float _height) const = 0;
	virtual void SetScissor(uint32_t _width, uint32_t _height) const = 0;

	virtual void AddMemoryBarrier0(IImage* _image) const = 0;
	virtual void AddMemoryBarrier1(IImage* _image) const = 0;
	virtual void Copy(IImage* _destination, IImage* _source) const = 0;
	virtual void Copy(IImage* _destination, IBuffer* _source) const = 0;

	virtual void BindVertexBuffer(IBuffer* const _vertexBuffer) const = 0;
	virtual void BindIndexBuffer(IBuffer* const _indexBuffer) const = 0;
	
	virtual void BindConstants(IPipeline* _pipeline, uint32_t _offset, uint32_t _size, void* _data) const = 0;

	virtual void Draw(uint32_t _vertexCount) const = 0;
	virtual void DrawIndexed(uint32_t _indexCount) const = 0;
	virtual void Dispatch(uint32_t _groupCountX, uint32_t _groupCountY, uint32_t _groupCountZ) const = 0;

	virtual VKCommandBuffer* const CastToVK() { return nullptr; }
	virtual DX12CommandBuffer* CastToDX12() { return nullptr; }
	virtual OGLCommandBuffer* CastToOGL() { return nullptr; }
};

#endif //HONOU_RENDER_ICOMMANDBUFFER_H
