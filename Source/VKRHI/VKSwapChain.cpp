#include <VKRHI/VKSwapChain.h>

#include <VKRHI/VKDevice.h>
#include <VKRHI/VKSurface.h>
#include <Helpers/Image.h>
#include <VKRHI/VKImage.h>

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
    VkSwapchainCreateInfoKHR swapChainCI{
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
    p_SwapChainImages.resize(imageCount);
    m_ImageSemaphores.resize(imageCount);
    m_ImageFences.resize(imageCount);
    vkGetSwapchainImagesKHR(lDevice, m_SwapChain, &imageCount, scImages.data());

    const ImageDescription desc{
        .width = scExtent.width,
        .height = scExtent.height,
        .format = scFormat
    };

    // TODO: Rework

    const VkSemaphoreCreateInfo semaphoreCI{
        .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO
    };

    const VkFenceCreateInfo fenceCI{
        .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
        .flags = VK_FENCE_CREATE_SIGNALED_BIT
    };

    for (size_t i = 0; i < imageCount; ++i)
    {
        VKImage* image = new VKImage();
        image->CreateImageInternal(_device, scImages[i], desc);
        p_SwapChainImages[i] = image;
        
        vkCreateSemaphore(lDevice, &semaphoreCI, nullptr, &m_ImageSemaphores[i]);
        vkCreateFence(lDevice, &fenceCI, nullptr, &m_ImageFences[i]);
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

        vkDestroyFence(lDevice, m_ImageFences[i], nullptr);
        vkDestroySemaphore(lDevice, m_ImageSemaphores[i], nullptr);
    }

    m_ImageFences.clear();
    m_ImageSemaphores.clear();
    p_SwapChainImages.clear();

    vkDestroySwapchainKHR(lDevice, m_SwapChain, nullptr);
}

uint32_t VKSwapChain::GetNextSwapChainImage(IDevice* _device, uint32_t _frameIndex)
{
    const VkDevice lDevice = _device->CastToVK()->GetLogicalDevice();

    vkWaitForFences(lDevice, 1, &m_ImageFences[_frameIndex], true, UINT64_MAX);
    vkResetFences(lDevice, 1, &m_ImageFences[_frameIndex]);

    vkAcquireNextImageKHR(lDevice, m_SwapChain, UINT64_MAX, m_ImageSemaphores[_frameIndex], VK_NULL_HANDLE, &p_CurrentImageIndex);
    return p_CurrentImageIndex;
}

void VKSwapChain::Present(IDevice* _device)
{
    const VkDevice lDevice = _device->CastToVK()->GetLogicalDevice();

    VkPresentInfoKHR presentInfo{
        .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
        .waitSemaphoreCount = 1,
        .pWaitSemaphores = VK_NULL_HANDLE,
        .swapchainCount = 1,
        .pSwapchains = &m_SwapChain,
        .pImageIndices = &p_CurrentImageIndex
    };

    vkQueuePresentKHR(VK_NULL_HANDLE, &presentInfo);
}
