#ifndef HONOU_RHI_VKFENCE_H
#define HONOU_RHI_VKFENCE_H

#include <RHI/IFence.h>

#include <vulkan/vulkan.h>

class VKFence : public IFence
{
public:
	~VKFence() override final = default;

	void CreateFence(IDevice* _device, bool _signaled = true) override final;
	void DestroyFence(IDevice* _device) override final;

	void Wait(IDevice* _device) override final;
	void Reset(IDevice* _device) override final;

	VKFence* CastToVK() override final { return this; }
	DX12Fence* CastToDX12() override final { return nullptr; }
	OGLFence* CastToOGL() override final { return nullptr; }

	[[nodiscard]] VkFence const GetRaw() const { return m_Fence; }

private:

	VkFence m_Fence = VK_NULL_HANDLE;
};

#endif //HONOU_RHI_VKFENCE_H