#include <DX12RHI/DX12Queue.h>

#include <DX12RHI/DX12Device.h>

void DX12Queue::CreateQueue(IDevice* _device, Type _queueType)
{
	ID3D12Device* device = _device->CastToDX12()->GetRaw().Get();

	const D3D12_COMMAND_QUEUE_DESC cmdQueueDesc{
		.Type = MapToDX12Type(_queueType),
		.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE
	};

	const HRESULT result = device->CreateCommandQueue(&cmdQueueDesc, IID_PPV_ARGS(&m_Queue));
	if (FAILED(result))
	{
	}

	_device->SetGraphicsQueue(this);
}

void DX12Queue::SubmitCommandBuffer(ICommandBuffer* _commandBuffer)
{
	ID3D12CommandList* cmdLists[] = {
		nullptr
	};

	m_Queue->ExecuteCommandLists(1, cmdLists);
}

void DX12Queue::SubmitCommandBuffer(ICommandBuffer* _commandBuffer, IFence* _fence, ISyncPoint* _waitPoint, ISyncPoint* _signalPoint)
{}

void DX12Queue::WaitIdle()
{
}