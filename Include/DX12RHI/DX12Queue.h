#ifndef HONOU_RHI_DX12QUEUE_H
#define HONOU_RHI_DX12QUEUE_H

#include <RHI/IQueue.h>

#include <wrl.h>
#include <d3d12.h>

class DX12Queue : public IQueue
{
public:
	~DX12Queue() override final = default;

	void CreateQueue(IDevice* _device, Type _queueType) override final;

	void SubmitCommandBuffer(ICommandBuffer* _commandBuffer) override final;
	void SubmitCommandBuffer(ICommandBuffer* _commandBuffer, IFence* _fence, ISyncPoint* _waitPoint, ISyncPoint* _signalPoint) override final;

	void WaitIdle() override final;

	VKQueue* CastToVK() override final { return nullptr; }
	DX12Queue* CastToDX12() override final { return this; }
	OGLQueue* CastToOGL() override final { return nullptr; }

	ID3D12CommandQueue* GetRaw() { return m_Queue.Get(); }

private:
	
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> m_Queue = nullptr;
};

static inline D3D12_COMMAND_LIST_TYPE MapToDX12Type(const IQueue::Type _type)
{
	// TEMP WILL MOVE IN AN OTHER FILE
	switch (_type)
	{
	default:
	case IQueue::Type::QUEUE_TYPE_GRAPHICS: return D3D12_COMMAND_LIST_TYPE_DIRECT;
	case IQueue::Type::QUEUE_TYPE_COMPUTE: return D3D12_COMMAND_LIST_TYPE_COMPUTE;
	case IQueue::Type::QUEUE_TYPE_COPY: return D3D12_COMMAND_LIST_TYPE_COPY;
	}
}

#endif //HONOU_RHI_DX12QUEUE_H