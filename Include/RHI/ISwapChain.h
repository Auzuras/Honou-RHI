#ifndef HONOU_RHI_ISWAPCHAIN_H
#define HONOU_RHI_ISWAPCHAIN_H

class VKSwapChain;
class DX12SwapChain;
class OGLSwapChain;

class IDevice;
class ISurface;
class IImage;
class IQueue;
class IFence;
class ISyncPoint;

#include <vector>

class ISwapChain
{
public:

    struct Description
    {
    public:
        uint32_t width      =   0u;
        uint32_t height     =   0u;
        ISurface* surface   =   nullptr;
    };

public:
    virtual ~ISwapChain() = default;

    virtual void CreateSwapChain(IDevice* _device, const Description& _description) = 0;
    virtual void RecreateSwapChain(IDevice* _device, const Description& _description) = 0;
    virtual void DestroySwapChain(IDevice* _device) = 0;

    virtual uint32_t GetNextSwapChainImage(IDevice* _device, uint32_t _frameIndex) = 0;
    virtual void Present(IDevice* _device, IQueue* _presentQueue, ISyncPoint* _syncPoint) = 0;

    ISyncPoint* GetImageSyncPoint(uint32_t _frameIndex) const { return p_SwapChainImageSyncPoint[_frameIndex]; }
    IFence* GetFence(uint32_t _frameIndex) const { return p_SwapChainImagesFences[_frameIndex]; }
    ISyncPoint* GetRenderSyncPoint() const { return p_SwapChainRenderSyncPoint[p_CurrentImageIndex]; }
    IImage* GetCurrentImage() const { return p_SwapChainImages[p_CurrentImageIndex]; }

    virtual VKSwapChain* CastToVK() { return nullptr; }
    virtual DX12SwapChain* CastToDX12() { return nullptr; }
    virtual OGLSwapChain* CastToOGL() { return nullptr; }

public:

    static const uint32_t MAX_FRAMES_IN_FLIGHT = 2;

protected:

    uint32_t p_CurrentImageIndex = 0;
    uint32_t p_SwapChainImagesCount = 0;

    std::vector<IImage*> p_SwapChainImages = std::vector<IImage*>();
    std::vector<IFence*> p_SwapChainImagesFences = std::vector<IFence*>();
    std::vector<ISyncPoint*> p_SwapChainImageSyncPoint = std::vector<ISyncPoint*>();
    std::vector<ISyncPoint*> p_SwapChainRenderSyncPoint = std::vector<ISyncPoint*>();
};

#endif //HONOU_RHI_ISWAPCHAIN_H