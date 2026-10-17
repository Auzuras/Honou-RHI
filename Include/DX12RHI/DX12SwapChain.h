#ifndef HONOU_RHI_DX12SWAPCHAIN_H
#define HONOU_RHI_DX12SWAPCHAIN_H

#include <RHI/ISwapChain.h>

#include <wrl.h>
#include <dxgi1_4.h>

class DX12SwapChain : public ISwapChain
{
public:
    ~DX12SwapChain() override final = default;

    void CreateSwapChain(IInstance* _instance, IDevice* _device, const Description& _description) override final;
    void RecreateSwapChain(IDevice* _device, const Description& _description) override final;
    void DestroySwapChain(IInstance* _instance, IDevice* _device) override final;

    uint32_t GetNextSwapChainImage(IDevice* _device, uint32_t _frameIndex) override final;
    void Present(IDevice* _device, IQueue* _presentQueue, ISyncPoint* _syncPoint) override final;

    VKSwapChain* CastToVK() override final { return nullptr; }
    DX12SwapChain* CastToDX12() override final { return this; }
    OGLSwapChain* CastToOGL() override final { return nullptr; }

private:

    void CreateSwapChainImages(IDevice* _device, uint32_t _width, uint32_t _height);
    void DestroySwapChainImages(IDevice* _device);

    void CreateSwapChainSyncObjects(IDevice* _device);
    void DestroySwapChainSyncObjects(IDevice* _device);

    //DXGI_SWAP_CHAIN_DESC1 CreateSwapChainInfo(VkDevice _device, VkPhysicalDevice _physicalDevice, VkSurfaceKHR _surface, uint32_t _width, uint32_t _height);

private:

    Microsoft::WRL::ComPtr<IDXGISwapChain3> m_SwapChain = nullptr;
};

#endif //HONOU_RHI_DX12SWAPCHAIN_H