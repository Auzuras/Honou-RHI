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
class IDescriptor;

class ICommandBuffer
{
public:

	enum class Layout : uint64_t
	{
		UNDEFINED = 0,

		RENDER_TARGET = 1,
		SHADER_READ = 2,
		UNORDERED_ACCESS = 3,

		TRANSFER_SRC = 4,
		TRANSFER_DST = 5,

		PRESENT = 6
	};

	enum class PipelineStage : uint64_t
	{
		NONE = 0,

		VERTEX_SHADER = 1ull << 0,
		FRAGMENT_SHADER = 1ull << 1,

		COLOR_ATTACHMENT = 1ull << 2,

		TRANSFER = 1ull << 3
	};

	enum class Access : uint64_t
	{
		NONE = 0,

		SHADER_READ = 1ull << 0,
		SHADER_WRITE = 1ull << 1,

		COLOR_ATTACHMENT_READ = 1ull << 2,
		COLOR_ATTACHMENT_WRITE = 1ull << 3,

		TRANSFER_READ = 1ull << 4,
		TRANSFER_WRITE = 1ull << 5,
	};

	struct MemoryBarrierInfo
	{
		struct SyncScope
		{
			PipelineStage stages;
			Access access;
		};

		Layout oldLayout = Layout::UNDEFINED;
		Layout newLayout = Layout::UNDEFINED;

		SyncScope syncSource;
		SyncScope syncDest;
	};

	virtual ~ICommandBuffer() = default;

	virtual void Open() const = 0;
	virtual void Close() const = 0;

	virtual void Reset() const = 0;

	virtual void BeginRendering(IImage* _image, uint32_t _width, uint32_t _height) const = 0;
	virtual void EndRendering() const = 0;

	virtual void BindPipeline(IPipeline* _pipeline) const = 0;

	virtual void SetViewport(float _width, float _height) const = 0;
	virtual void SetScissor(uint32_t _width, uint32_t _height) const = 0;

	virtual void AddMemoryBarrier(IImage* _image, MemoryBarrierInfo _memoryBarrier) const = 0;
	virtual void Copy(IImage* _destination, IImage* _source) const = 0;
	virtual void Copy(IImage* _destination, IBuffer* _source) const = 0;
	virtual void Copy(IBuffer* _source, IImage* _destination) const = 0;
	virtual void Copy(IBuffer* _source, IBuffer* _destination) const = 0;

	virtual void BindVertexBuffer(IBuffer* const _vertexBuffer) const = 0;
	virtual void BindIndexBuffer(IBuffer* const _indexBuffer) const = 0;
	
	virtual void BindConstants(IPipeline* _pipeline, uint32_t _offset, uint32_t _size, void* _data) const = 0;
	virtual void BindDescriptor(IPipeline* _pipeline, IDescriptor* _descriptor, uint32_t _set) const = 0;

	virtual void Draw(uint32_t _vertexCount) const = 0;
	virtual void DrawIndexed(uint32_t _indexCount) const = 0;
	virtual void Dispatch(uint32_t _groupCountX, uint32_t _groupCountY, uint32_t _groupCountZ) const = 0;

	virtual VKCommandBuffer* const CastToVK() { return nullptr; }
	virtual DX12CommandBuffer* CastToDX12() { return nullptr; }
	virtual OGLCommandBuffer* CastToOGL() { return nullptr; }
};

#endif //HONOU_RENDER_ICOMMANDBUFFER_H
