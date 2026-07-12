#include <VKRHI/VKSwapChain.h>

#include <VKRHI/VKDevice.h>
#include <VKRHI/VKSurface.h>
#include <Helpers/Image.h>
#include <VKRHI/VKImage.h>
#include <VKRHI/VKQueue.h>
#include <VKRHI/VKFence.h>
#include <VKRHI/VKSyncPoint.h>

void VKSwapChain::CreateSwapChain(IDevice* _device, ISurface* _surface)
{
    const VkSurfaceKHR surface = _surface->CastToVK()->GetRaw();
    const VkPhysicalDevice pDevice = _device->CastToVK()->GetPhysicalDevice();
    const VkDevice lDevice = _device->CastToVK()->GetLogicalDevice();

    VkSurfaceCapabilitiesKHR surfaceCaps{};
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(pDevice, surface, &surfaceCaps);

    VkExtent2D scExtent{ surfaceCaps.currentExtent };
    if (surfaceCaps.currentExtent.width == 0xFFFFFFFF)
        return;

    const TextureFormat scFormat = TextureFormat::HONOU_FORMAT_B8G8R8A8_UNORM;

    const VkFormat imageFormat = MapToVkFormat(scFormat);
    const VkSwapchainCreateInfoKHR swapChainCI{
        .sType              =   VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,

        .surface            =   surface,
        .minImageCount      =   surfaceCaps.minImageCount,
        .imageFormat        =   imageFormat,
        .imageColorSpace    =   VK_COLOR_SPACE_SRGB_NONLINEAR_KHR,

        .imageExtent{
            .width          =   scExtent.width,
            .height         =   scExtent.height,
        },

        .imageArrayLayers   =   1,
        .imageUsage         =   VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
        .preTransform       =   VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR,
        .compositeAlpha     =   VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
        .presentMode        =   VK_PRESENT_MODE_FIFO_KHR
    };

    vkCreateSwapchainKHR(lDevice, &swapChainCI, nullptr, &m_SwapChain);

    uint32_t imageCount = 0;
    std::vector<VkImage> scImages = std::vector<VkImage>();
    
    vkGetSwapchainImagesKHR(lDevice, m_SwapChain, &imageCount, nullptr);
    
    scImages.resize(imageCount);
    vkGetSwapchainImagesKHR(lDevice, m_SwapChain, &imageCount, scImages.data());

    const ImageDescription desc{
        .width = scExtent.width,
        .height = scExtent.height,
        .format = scFormat
    };

    // TODO: Rework

    for (size_t i = 0; i < imageCount; ++i)
    {
        VKImage* image = new VKImage();
        image->CreateImageInternal(_device, scImages[i], desc);
        p_SwapChainImages.push_back(image);

        VKSyncPoint* renderSyncPoint = new VKSyncPoint();
        renderSyncPoint->CreateSyncPoint(_device);
        p_SwapChainRenderSyncPoint.push_back(renderSyncPoint);
    }

    for (size_t i = 0; i < ISwapChain::MAX_FRAMES_IN_FLIGHT; ++i)
    {
        VKFence* fence = new VKFence();
        fence->CreateFence(_device);
        p_SwapChainImagesFences.push_back(fence);

        VKSyncPoint* imageSyncPoint = new VKSyncPoint();
        imageSyncPoint->CreateSyncPoint(_device);
        p_SwapChainImageSyncPoint.push_back(imageSyncPoint);
    }
}

void VKSwapChain::DestroySwapChain(IDevice* _device)
{
    const VkDevice lDevice = _device->CastToVK()->GetLogicalDevice();

    // TODO: Rework
    for (size_t i = 0; i < p_SwapChainImages.size(); ++i)
    {
        p_SwapChainImages[i]->DestroyImage(_device);
        delete p_SwapChainImages[i];

        p_SwapChainRenderSyncPoint[i]->DestroySyncPoint(_device);
        delete p_SwapChainRenderSyncPoint[i];
    }

    for (size_t i = 0; i < ISwapChain::MAX_FRAMES_IN_FLIGHT; ++i)
    {
        p_SwapChainImagesFences[i]->DestroyFence(_device);
        delete p_SwapChainImagesFences[i];

        p_SwapChainImageSyncPoint[i]->DestroySyncPoint(_device);
        delete p_SwapChainImageSyncPoint[i];
    }

    p_SwapChainRenderSyncPoint.clear();
    p_SwapChainImageSyncPoint.clear();
    p_SwapChainImagesFences.clear();
    p_SwapChainImages.clear();

    vkDestroySwapchainKHR(lDevice, m_SwapChain, nullptr);
}

uint32_t VKSwapChain::GetNextSwapChainImage(IDevice* _device, uint32_t _frameIndex)
{
    const VkDevice lDevice = _device->CastToVK()->GetLogicalDevice();
    const VkSemaphore imageSemaphore = p_SwapChainImageSyncPoint[_frameIndex]->CastToVK()->GetRaw();

    p_SwapChainImagesFences[_frameIndex]->Wait(_device);
    p_SwapChainImagesFences[_frameIndex]->Reset(_device);

    vkAcquireNextImageKHR(lDevice, m_SwapChain, UINT64_MAX, imageSemaphore, VK_NULL_HANDLE, &p_CurrentImageIndex);
    return p_CurrentImageIndex;
}

void VKSwapChain::Present(IDevice* _device, IQueue* _presentQueue, ISyncPoint* _syncPoint)
{
    const VkDevice lDevice = _device->CastToVK()->GetLogicalDevice();
    const VkSemaphore semaphore = _syncPoint->CastToVK()->GetRaw();
    const VkQueue queue = _presentQueue->CastToVK()->GetRaw();

    VkPresentInfoKHR presentInfo{
        .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
        .waitSemaphoreCount = 1,
        .pWaitSemaphores = &semaphore,
        .swapchainCount = 1,
        .pSwapchains = &m_SwapChain,
        .pImageIndices = &p_CurrentImageIndex
    };

    vkQueuePresentKHR(queue, &presentInfo);
}
