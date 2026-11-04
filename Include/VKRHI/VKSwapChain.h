#ifndef HONOU_RHI_VKSWAPCHAIN_H
#define HONOU_RHI_VKSWAPCHAIN_H

#include <RHI/ISwapChain.h>

#include <vulkan/vulkan.h>

#include <array>

class VKSwapChain : public ISwapChain
{
public:
    ~VKSwapChain() override final = default;

    void CreateSwapChain(IInstance* _instance, IDevice* _device, const Description& _description) override final;
    void RecreateSwapChain(IDevice* _device, const Description& _description) override final;
    void DestroySwapChain(IInstance* _instance, IDevice* _device) override final;

    uint32_t Acquire(IDevice* _device, uint32_t _frameIndex) override final;
    void Present(IDevice* _device, IQueue* _presentQueue) override final;

    VKSwapChain* CastToVK() override final { return this; }
    DX12SwapChain* CastToDX12() override final { return nullptr; }
    OGLSwapChain* CastToOGL() override final { return nullptr; }

private:

    void CreateSwapChainImages(IDevice* _device, uint32_t _width, uint32_t _height);
    void DestroySwapChainImages(IDevice* _device);

    void CreateSwapChainSyncObjects(IDevice* _device);
    void DestroySwapChainSyncObjects(IDevice* _device);

    VkSwapchainCreateInfoKHR CreateSwapChainInfo(VkDevice _device, VkPhysicalDevice _physicalDevice, VkSurfaceKHR _surface, uint32_t _width, uint32_t _height);

private:

    VkSwapchainKHR m_SwapChain = VK_NULL_HANDLE;
    VkSurfaceKHR m_Surface = VK_NULL_HANDLE;

    std::array<VkSemaphore, MAX_FRAMES_IN_FLIGHT> m_ImageSemaphores = {};
};

#endif //HONOU_RHI_VKSWAPCHAIN_H