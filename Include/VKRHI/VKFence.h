#ifndef HONOU_RHI_VKFENCE_H
#define HONOU_RHI_VKFENCE_H

#include <RHI/IFence.h>

#include <vulkan/vulkan.h>

class VKFence : public IFence
{
public:
	~VKFence() override final = default;

	void CreateFence(IDevice* _device) override final;
	void DestroyFence(IDevice* _device) override final;

	void SignalGPU(IDevice* _device, uint64_t _value) override final;
	void WaitCPU(IDevice* _device, uint64_t _value) override final;
	uint64_t GetCompletedValue(IDevice* _device) override final;

	VKFence* CastToVK() override final { return this; }
	DX12Fence* CastToDX12() override final { return nullptr; }
	OGLFence* CastToOGL() override final { return nullptr; }

	[[nodiscard]] VkSemaphore const GetRaw() const { return m_Semaphore; }

private:

	VkSemaphore m_Semaphore = VK_NULL_HANDLE;
};

#endif //HONOU_RHI_VKFENCE_H