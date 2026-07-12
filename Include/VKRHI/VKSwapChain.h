#ifndef HONOU_RENDER_VKSWAPCHAIN_H
#define HONOU_RENDER_VKSWAPCHAIN_H

#include <RHI/ISwapChain.h>

#include <vulkan/vulkan.h>

class VKSwapChain : public ISwapChain
{
public:
    ~VKSwapChain() override final = default;

    void CreateSwapChain(IDevice* _device, ISurface* _surface) override final;
    void DestroySwapChain(IDevice* _device) override final;

    uint32_t GetNextSwapChainImage(IDevice* _device, uint32_t _frameIndex) override final;
    void Present(IDevice* _device, IQueue* _presentQueue) override final;

    VKSwapChain* CastToVK() override final { return this; }
    DX12SwapChain* CastToDX12() override final { return nullptr; }
    OGLSwapChain* CastToOGL() override final { return nullptr; }

private:

    VkSwapchainKHR m_SwapChain = VK_NULL_HANDLE;

    std::vector<VkSemaphore> m_ImageSemaphores = std::vector<VkSemaphore>();
};

#endif //HONOU_RENDER_VKSWAPCHAIN_H