#ifndef HONOU_RHI_DX12COMMANDALLOCATOR_H
#define HONOU_RHI_DX12COMMANDALLOCATOR_H

class VKCommandAllocator;
class DX12CommandAllocator;
class OGLCommandAllocator;

#include <RHI/ICommandAllocator.h>

#include <wrl.h>
#include <d3d12.h>

class DX12CommandAllocator : public ICommandAllocator
{
public:
	~DX12CommandAllocator() override final = default;

	void CreateCommandAllocator(IDevice* _device) override final;
	void DestroyCommandAllocator(IDevice* _device) override final;

	std::vector<ICommandBuffer*> AllocateCommandBuffers(IDevice* _device, uint32_t _count) override final;
	ICommandBuffer* AllocateCommandBuffer(IDevice* _device) override final;

	void FreeCommandBuffers(IDevice* _device, const std::vector<ICommandBuffer*>& _buffers) override final;
	void FreeCommandBuffer(IDevice* _device, ICommandBuffer* _buffer) override final;

	VKCommandAllocator* const CastToVK() { return nullptr; }
	DX12CommandAllocator* CastToDX12() { return this; }
	OGLCommandAllocator* CastToOGL() { return nullptr; }

private:

	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> m_CommandAllocator = nullptr;
};

#endif //HONOU_RHI_DX12COMMANDALLOCATOR_H
