#ifndef HONOU_RHI_DX12COMMANDBUFFER_H
#define HONOU_RHI_DX12COMMANDBUFFER_H

class VKCommandBuffer;
class DX12CommandBuffer;
class OGLCommandBuffer;

class VKCommandAllocator;

#include <RHI/ICommandBuffer.h>

#define NOMINMAX

#include <wrl.h>
#include <d3d12.h>

class DX12CommandBuffer : public ICommandBuffer
{
public:
	~DX12CommandBuffer() override final = default;

	ID3D12GraphicsCommandList1* GetRaw() const { return m_CommandBuffer.Get(); }

	void Open() const override final;
	void Close() const override final;

	void Reset() const override final;

	void BeginRendering(IImage* _image, IImage* _depth, uint32_t _width, uint32_t _height) const override final;
	void EndRendering() const override final;

	void BindPipeline(IPipeline* _pipeline) const override final;

	void SetViewport(float _width, float _height) const override final;
	void SetScissor(uint32_t _width, uint32_t _height) const override final;

	void AddMemoryBarrier(IImage* _image, MemoryBarrierInfo _memoryBarrier) const override final;
	void Copy(IImage* _destination, IImage* _source) const override final;
	void Copy(IImage* _destination, IBuffer* _source) const override final;
	void Copy(IBuffer* _source, IImage* _destination) const override final;
	void Copy(IBuffer* _source, IBuffer* _destination) const override final;

	void BindVertexBuffer(IBuffer* const _vertexBuffer) const override final;
	void BindIndexBuffer(IBuffer* const _indexBuffer) const override final;

	void BindConstants(IPipeline* _pipeline, uint32_t _offset, uint32_t _size, void* _data) const override final;
	void BindDescriptor(IPipeline* _pipeline, IDescriptor* _descriptor, uint32_t _set) const override final;

	void Draw(uint32_t _vertexCount) const override final;
	void DrawIndexed(uint32_t _indexCount) const override final;
	void Dispatch(uint32_t _groupCountX, uint32_t _groupCountY, uint32_t _groupCountZ) const override final;

	void DrawIndexedIndirect(IBuffer* _buffer, uint32_t _offset) const override final;
	void DispatchIndirect(IBuffer* _buffer, uint32_t _offset) const override final;

	void AddLabel(const char* _name, const std::array<float, 4>& _color) const override final;
	void EndLabel() const override final;

	VKCommandBuffer* const CastToVK() { return nullptr; }
	DX12CommandBuffer* CastToDX12() { return this; }
	OGLCommandBuffer* CastToOGL() { return nullptr; }

private:
	friend class VKCommandAllocator;

	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList1> m_CommandBuffer;
};

#endif //HONOU_RHI_DX12COMMANDBUFFER_H
