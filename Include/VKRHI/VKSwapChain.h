#ifndef HONOU_RENDER_VKSWAPCHAIN_H
#define HONOU_RENDER_VKSWAPCHAIN_H

#include <RHI/ISwapChain.h>

#include <vulkan/vulkan.h>

class VKSwapChain : public ISwapChain
{
public:
    ~VKSwapChain() override final = default;

    void CreateSwapChain(IDevice* _device) override final;
    void DestroySwapChain(IDevice* _device) override final;

    VKSwapChain* CastToVK() override final { return this; }
    DX12SwapChain* CastToDX12() override final { return nullptr; }
    OGLSwapChain* CastToOGL() override final { return nullptr; }

private:

    VkSwapchainKHR m_SwapChain = VK_NULL_HANDLE;
};


#endif //HONOU_RENDER_VKSWAPCHAIN_H