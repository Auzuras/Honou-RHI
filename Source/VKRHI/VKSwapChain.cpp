#include <VKRHI/VKSwapChain.h>

#include <VKRHI/VKDevice.h>
#include <VKRHI/VKSurface.h>
#include <Helpers/Image.h>
#include <VKRHI/VKImage.h>
#include <VKRHI/VKQueue.h>
#include <VKRHI/VKFence.h>
#include <VKRHI/VKSyncPoint.h>

#include <string>

void VKSwapChain::CreateSwapChain(IDevice* _device, const Description& _description)
{
    const VkSurfaceKHR surface = _description.surface->CastToVK()->GetRaw();
    const VkPhysicalDevice pDevice = _device->CastToVK()->GetPhysicalDevice();
    const VkDevice lDevice = _device->CastToVK()->GetLogicalDevice();

    const VkSwapchainCreateInfoKHR swapChainCI = CreateSwapChainInfo(lDevice, pDevice, surface, _description.width, _description.height);

    vkCreateSwapchainKHR(lDevice, &swapChainCI, nullptr, &m_SwapChain);

    CreateSwapChainImages(_device, _description.width, _description.height);

    CreateSwapChainSyncObjects(_device);
}

void VKSwapChain::RecreateSwapChain(IDevice* _device, const Description& _description)
{
    const VkSurfaceKHR surface = _description.surface->CastToVK()->GetRaw();
    const VkPhysicalDevice pDevice = _device->CastToVK()->GetPhysicalDevice();
    const VkDevice lDevice = _device->CastToVK()->GetLogicalDevice();

    VkSwapchainCreateInfoKHR swapChainCI = CreateSwapChainInfo(lDevice, pDevice, surface, _description.width, _description.height);
    swapChainCI.oldSwapchain = m_SwapChain;

    VkSwapchainKHR swapChain = VK_NULL_HANDLE;
    vkCreateSwapchainKHR(lDevice, &swapChainCI, nullptr, &swapChain);

    DestroySwapChainImages(_device);

    vkDestroySwapchainKHR(lDevice, m_SwapChain, nullptr);

    m_SwapChain = swapChain;

    CreateSwapChainImages(_device, _description.width, _description.height);
}

void VKSwapChain::DestroySwapChain(IDevice* _device)
{
    const VkDevice lDevice = _device->CastToVK()->GetLogicalDevice();

    DestroySwapChainImages(_device);

    DestroySwapChainSyncObjects(_device);

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

void VKSwapChain::CreateSwapChainImages(IDevice* _device, uint32_t _width, uint32_t _height)
{
    const VkDevice lDevice = _device->CastToVK()->GetLogicalDevice();

    std::vector<VkImage> scImages = std::vector<VkImage>();

    vkGetSwapchainImagesKHR(lDevice, m_SwapChain, &p_SwapChainImagesCount, nullptr);
    scImages.resize(p_SwapChainImagesCount);
    vkGetSwapchainImagesKHR(lDevice, m_SwapChain, &p_SwapChainImagesCount, scImages.data());

    const ImageDescription desc{
        .width = _width,
        .height = _height,
        .format = TextureFormat::HONOU_FORMAT_B8G8R8A8_UNORM
    };

    // TODO: Rework
    for (size_t i = 0; i < p_SwapChainImagesCount; ++i)
    {
        VKSyncPoint* renderSyncPoint = new VKSyncPoint();
        renderSyncPoint->CreateSyncPoint(_device);
        p_SwapChainRenderSyncPoint.push_back(renderSyncPoint);

        VKImage* image = new VKImage();
        image->CreateImageInternal(_device, scImages[i], desc);
        std::string name = "Hounou RHI - SwapChain Image " + std::to_string(i);
        image->SetName(_device, name.c_str());
        p_SwapChainImages.push_back(image);
    }
}

void VKSwapChain::DestroySwapChainImages(IDevice* _device)
{
    for (size_t i = 0; i < p_SwapChainImagesCount; ++i)
    {
        p_SwapChainImages[i]->DestroyImage(_device);
        delete p_SwapChainImages[i];

        p_SwapChainRenderSyncPoint[i]->DestroySyncPoint(_device);
        delete p_SwapChainRenderSyncPoint[i];
    }

    p_SwapChainRenderSyncPoint.clear();
    p_SwapChainImages.clear();
}

void VKSwapChain::CreateSwapChainSyncObjects(IDevice* _device)
{
    // TODO: Rework
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

void VKSwapChain::DestroySwapChainSyncObjects(IDevice* _device)
{
    // TODO: Rework
    for (size_t i = 0; i < ISwapChain::MAX_FRAMES_IN_FLIGHT; ++i)
    {
        p_SwapChainImagesFences[i]->DestroyFence(_device);
        delete p_SwapChainImagesFences[i];

        p_SwapChainImageSyncPoint[i]->DestroySyncPoint(_device);
        delete p_SwapChainImageSyncPoint[i];
    }

    p_SwapChainImageSyncPoint.clear();
    p_SwapChainImagesFences.clear();
}

VkSwapchainCreateInfoKHR VKSwapChain::CreateSwapChainInfo(VkDevice _device, VkPhysicalDevice _physicalDevice, VkSurfaceKHR _surface, uint32_t _width, uint32_t _height)
{
    VkSurfaceCapabilitiesKHR surfaceCaps{};
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(_physicalDevice, _surface, &surfaceCaps);

    VkExtent2D scExtent{ surfaceCaps.currentExtent };
    if (surfaceCaps.currentExtent.width == 0xFFFFFFFF)
        return VkSwapchainCreateInfoKHR();

    const TextureFormat scFormat = TextureFormat::HONOU_FORMAT_B8G8R8A8_UNORM;
    const VkFormat imageFormat = MapToVkFormat(scFormat);

    VkSwapchainCreateInfoKHR swapChainCI{
        .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,

        .surface = _surface,
        .minImageCount = surfaceCaps.minImageCount,
        .imageFormat = imageFormat,
        .imageColorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR,

        .imageExtent{
            .width = scExtent.width,
            .height = scExtent.height,
        },

        .imageArrayLayers = 1,
        .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT,
        .preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR,
        .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
        .presentMode = VK_PRESENT_MODE_IMMEDIATE_KHR
    };

    return swapChainCI;
}
