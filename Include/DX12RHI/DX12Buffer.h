#ifndef HONOU_RHI_DX12BUFFER_H
#define HONOU_RHI_DX12BUFFER_H

class VKBuffer;
class DX12Buffer;
class OGLBuffer;

#include <RHI/IBuffer.h>

#include <wrl.h>
#include <d3d12.h>

class DX12Buffer : public IBuffer
{
public:
	~DX12Buffer() override final = default;

	void CreateBuffer(IDevice* _device, const Description& _description) override final;
	void DestroyBuffer(IDevice* _device) override final;

	void SetName(IDevice* _device, const char* _name) override final;

	VKBuffer* const CastToVK() override final { return nullptr; }
	DX12Buffer* CastToDX12() override final { return this; }
	OGLBuffer* CastToOGL() override final { return nullptr; }

	ID3D12Resource* GetRaw() { return m_Buffer.Get(); }

private:

	Microsoft::WRL::ComPtr<ID3D12Resource> m_Buffer = nullptr;
	D3D12_VERTEX_BUFFER_VIEW m_BufferView = {};
};

#endif //HONOU_RHI_DX12BUFFER_H
