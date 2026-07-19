#ifndef HONOU_RHI_ICOMMANDBUFFER_H
#define HONOU_RHI_ICOMMANDBUFFER_H

#include <cstdint>
#include <array>

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
		UNDEFINED				= 0,	// VK_IMAGE_LAYOUT_UNDEFINED

		COLOR_OPTIMAL			= 1,	// VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL
		DEPTH_OPTIMAL			= 2,	// VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL
		DEPTH_STENCIL_OPTIMAL	= 3,	// VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL
		SHADER_READ				= 4,	// VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
		UNORDERED_ACCESS		= 5,	// VK_IMAGE_LAYOUT_GENERAL

		TRANSFER_SRC			= 6,	// VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL
		TRANSFER_DST			= 7,	// VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL

		PRESENT					= 8		// VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
	};

	enum class PipelineStage : uint64_t
	{
		NONE					= 0,			// VK_PIPELINE_STAGE_2_NONE

		VERTEX_SHADER			= 1ull << 0,	// VK_PIPELINE_STAGE_2_VERTEX_SHADER_BIT
		FRAGMENT_SHADER			= 1ull << 1,	// VK_PIPELINE_STAGE_2_FRAGMENT_SHADER_BIT
		COMPUTE_SHADER			= 1ull << 2,	// VK_PIPELINE_STAGE_2_COMPUTE_SHADER_BIT
		EARLY_FRAGMENT_TEST		= 1ull << 3,	// VK_PIPELINE_STAGE_2_EARLY_FRAGMENT_TESTS_BIT
		LATE_FRAGMENT_TEST		= 1ull << 4,	// VK_PIPELINE_STAGE_2_LATE_FRAGMENT_TESTS_BIT

		COLOR_ATTACHMENT		= 1ull << 5,	// VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT

		TRANSFER				= 1ull << 6		// VK_PIPELINE_STAGE_2_TRANSFER_BIT
	};

	enum class Access : uint64_t
	{
		NONE							= 0,			// VK_ACCESS_2_NONE

		SHADER_READ						= 1ull << 0,	// VK_ACCESS_2_SHADER_READ_BIT
		SHADER_WRITE					= 1ull << 1,	// VK_ACCESS_2_SHADER_WRITE_BIT

		COLOR_ATTACHMENT_READ			= 1ull << 2,	// VK_ACCESS_2_COLOR_ATTACHMENT_READ_BIT
		COLOR_ATTACHMENT_WRITE			= 1ull << 3,	// VK_ACCESS_2_COLOR_ATTACHMENT_WRITE_BIT

		DEPTH_STENCIL_ATTACHMENT_READ	= 1ull << 4,	// VK_ACCESS_2_DEPTH_STENCIL_ATTACHMENT_READ_BIT
		DEPTH_STENCIL_ATTACHMENT_WRITE	= 1ull << 5,	// VK_ACCESS_2_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT

		TRANSFER_READ					= 1ull << 6,	// VK_ACCESS_2_TRANSFER_READ_BIT
		TRANSFER_WRITE					= 1ull << 7,	// VK_ACCESS_2_TRANSFER_WRITE_BIT
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

	virtual void BeginRendering(IImage* _image, IImage* _depth, uint32_t _width, uint32_t _height) const = 0;
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

	virtual void AddLabel(const char* _name, const std::array<float, 4>& _color) const = 0;
	virtual void EndLabel() const = 0;

	virtual VKCommandBuffer* const CastToVK() { return nullptr; }
	virtual DX12CommandBuffer* CastToDX12() { return nullptr; }
	virtual OGLCommandBuffer* CastToOGL() { return nullptr; }
};

#endif //HONOU_RHI_ICOMMANDBUFFER_H
