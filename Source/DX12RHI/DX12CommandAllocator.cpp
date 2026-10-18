#include <DX12RHI/DX12CommandAllocator.h>

#include <DX12RHI/DX12Device.h>
#include <DX12RHI/DX12CommandBuffer.h>

void DX12CommandAllocator::CreateCommandAllocator(IDevice* _device)
{
	ID3D12Device* const device = _device->CastToDX12()->GetRaw();

	const HRESULT result = device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_CommandAllocator));

	if (FAILED(result))
	{

	}
}

void DX12CommandAllocator::DestroyCommandAllocator(IDevice* _device)
{
	m_CommandAllocator = nullptr;
}

std::vector<ICommandBuffer*> DX12CommandAllocator::AllocateCommandBuffers(IDevice* _device, uint32_t _count)
{
	ID3D12Device* const device = _device->CastToDX12()->GetRaw();

	// Temp allocation for now
	std::vector<ICommandBuffer*> commandBuffers = std::vector<ICommandBuffer*>(_count);
	for (size_t i = 0; i < _count; ++i)
	{
		DX12CommandBuffer* cmdBuffer = new DX12CommandBuffer();
		const HRESULT result = device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_CommandAllocator.Get(), nullptr, IID_PPV_ARGS(&cmdBuffer->m_CommandBuffer));
		
		if (FAILED(result))
		{

		}
		
		cmdBuffer->m_Allocator = this;

		commandBuffers[i] = cmdBuffer;

		cmdBuffer->Close();
	}

	return commandBuffers;
}

ICommandBuffer* DX12CommandAllocator::AllocateCommandBuffer(IDevice* _device)
{
	return AllocateCommandBuffers(_device, 1)[0];
}

void DX12CommandAllocator::FreeCommandBuffers(IDevice* _device, const std::vector<ICommandBuffer*>& _buffers)
{
	for (size_t i = 0; i < _buffers.size(); ++i)
		FreeCommandBuffer(_device, _buffers[i]);
}

void DX12CommandAllocator::FreeCommandBuffer(IDevice* _device, ICommandBuffer* _buffer)
{
	_buffer->CastToDX12()->m_CommandBuffer = nullptr;
	delete _buffer;
}