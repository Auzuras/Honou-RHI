#ifndef HONOU_RENDER_ISWAPCHAIN_H
#define HONOU_RENDER_ISWAPCHAIN_H

class VKSwapChain;
class DX12SwapChain;
class OGLSwapChain;

class IInstance;
class IDevice;

class ISwapChain
{
public:
    virtual ~ISwapChain() = default;

    virtual void CreateSwapChain(IInstance* _instance, IDevice* _device) = 0;
    virtual void DestroySwapChain(IDevice* _device) = 0;

    virtual VKSwapChain* CastToVK() { return nullptr; }
    virtual DX12SwapChain* CastToDX12() { return nullptr; }
    virtual OGLSwapChain* CastToOGL() { return nullptr; }
};

#endif //HONOU_RENDER_ISWAPCHAIN_H