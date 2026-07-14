#ifndef HONOU_RENDER_VKSWAPCHAIN_H
#define HONOU_RENDER_VKSWAPCHAIN_H

#include <RHI/ISwapChain.h>

#include <vulkan/vulkan.h>

class VKSwapChain : public ISwapChain
{
public:
    ~VKSwapChain() override final = default;

    void CreateSwapChain(IDevice* _device, ISurface* _surface, uint32_t _width, uint32_t _height) override final;
    void RecreateSwapChain(IDevice* _device, ISurface* _surface, uint32_t _width, uint32_t _height) override final;
    void DestroySwapChain(IDevice* _device) override final;

    uint32_t GetNextSwapChainImage(IDevice* _device, uint32_t _frameIndex) override final;
    void Present(IDevice* _device, IQueue* _presentQueue, ISyncPoint* _syncPoint) override final;

    VKSwapChain* CastToVK() override final { return this; }
    DX12SwapChain* CastToDX12() override final { return nullptr; }
    OGLSwapChain* CastToOGL() override final { return nullptr; }

private:

    VkSwapchainCreateInfoKHR CreateSwapChainHelper(VkDevice _device, VkPhysicalDevice _physicalDevice, VkSurfaceKHR _surface, uint32_t _width, uint32_t _height);

private:

    VkSwapchainKHR m_SwapChain = VK_NULL_HANDLE;
};

#endif //HONOU_RENDER_VKSWAPCHAIN_H