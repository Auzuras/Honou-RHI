#ifndef HONOU_RENDER_ISWAPCHAIN_H
#define HONOU_RENDER_ISWAPCHAIN_H

class VKSwapChain;
class DX12SwapChain;
class OGLSwapChain;

class IDevice;
class ISurface;
class IImage;
class IQueue;
class IFence;

#include <vector>

class ISwapChain
{
public:
    virtual ~ISwapChain() = default;

    virtual void CreateSwapChain(IDevice* _device, ISurface* _surface) = 0;
    virtual void DestroySwapChain(IDevice* _device) = 0;

    virtual uint32_t GetNextSwapChainImage(IDevice* _device, uint32_t _frameIndex) = 0;
    virtual void Present(IDevice* _device, IQueue* _presentQueue) = 0;

    virtual VKSwapChain* CastToVK() { return nullptr; }
    virtual DX12SwapChain* CastToDX12() { return nullptr; }
    virtual OGLSwapChain* CastToOGL() { return nullptr; }

public:

    static const uint32_t MAX_FRAMES_IN_FLIGHT = 2;

protected:

    uint32_t p_CurrentImageIndex = 0;

    std::vector<IImage*> p_SwapChainImages = std::vector<IImage*>();
    std::vector<IFence*> p_SwapChainImagesFences = std::vector<IFence*>();
};

#endif //HONOU_RENDER_ISWAPCHAIN_H