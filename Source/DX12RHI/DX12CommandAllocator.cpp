#include <DX12RHI/DX12CommandAllocator.h>

#include <DX12RHI/DX12Device.h>

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

	//VkCommandBufferAllocateInfo cbAllocCI{
	//	.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
	//	.commandPool = m_CommandAllocator,
	//	.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
	//	.commandBufferCount = _count
	//};

	//std::vector<VkCommandBuffer> vkCmdBuffers{ _count };
	//vkAllocateCommandBuffers(lDevice, &cbAllocCI, vkCmdBuffers.data());

	//// Temp allocation for now
	//std::vector<ICommandBuffer*> commandBuffers = std::vector<ICommandBuffer*>(_count);
	//for (size_t i = 0; i < _count; ++i)
	//{
	//	VKCommandBuffer* cmdBuffer = new VKCommandBuffer();
	//	cmdBuffer->m_CommandBuffer = vkCmdBuffers[i];
	//	commandBuffers[i] = cmdBuffer;
	//}

	//return commandBuffers;

	return std::vector<ICommandBuffer*>{};
}

ICommandBuffer* DX12CommandAllocator::AllocateCommandBuffer(IDevice* _device)
{
	return nullptr;
}

void DX12CommandAllocator::FreeCommandBuffers(IDevice* _device, const std::vector<ICommandBuffer*>& _buffers)
{}

void DX12CommandAllocator::FreeCommandBuffer(IDevice* _device, ICommandBuffer* _buffer)
{}