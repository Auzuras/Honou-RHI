#ifndef HONOU_RHI_ISWAPCHAIN_H
#define HONOU_RHI_ISWAPCHAIN_H

class VKSwapChain;
class DX12SwapChain;
class OGLSwapChain;

class IInstance;
class IDevice;
class ISurface;
class IImage;
class IQueue;
class IFence;
class ISyncPoint;

#include <vector>
#include <array>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

class ISwapChain
{
public:

    struct Description
    {
    public:
        uint32_t width      =   0u;
        uint32_t height     =   0u;
//#ifdef WINDOWS
        HINSTANCE instance = NULL;
        HWND hwnd = NULL;
//#endif // WINDOWS
    };

public:
    virtual ~ISwapChain() = default;

    virtual void CreateSwapChain(IInstance* _instance, IDevice* _device, const Description& _description) = 0;
    virtual void RecreateSwapChain(IDevice* _device, const Description& _description) = 0;
    virtual void DestroySwapChain(IInstance* _instance, IDevice* _device) = 0;

    virtual uint32_t Acquire(IDevice* _device, uint32_t _frameIndex) = 0;
    virtual void Present(IDevice* _device, IQueue* _presentQueue, uint32_t _frameIndex) = 0;

    IImage* GetCurrentImage() const { return p_SwapChainImages[p_CurrentImageIndex]; }

    virtual VKSwapChain* CastToVK() { return nullptr; }
    virtual DX12SwapChain* CastToDX12() { return nullptr; }
    virtual OGLSwapChain* CastToOGL() { return nullptr; }

public:

    static const uint32_t MAX_FRAMES_IN_FLIGHT = 3;

protected:

    uint32_t p_CurrentImageIndex = 0;
    uint32_t p_SwapChainImagesCount = 0;

    std::vector<IImage*> p_SwapChainImages = {};
    
    IFence* p_SwapChainFence = nullptr;
    std::array<uint64_t, ISwapChain::MAX_FRAMES_IN_FLIGHT> p_FenceValues = {};
    uint64_t p_NextFenceValue = 0ull;
};

#endif //HONOU_RHI_ISWAPCHAIN_H