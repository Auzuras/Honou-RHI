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

    for (size_t i = 0; i < imageCount; ++i)
    {
        VKImage* image = new VKImage();
        image->CreateImageInternal(_device, scImages[i], desc);
        p_SwapChainImages[i] = image;
        
        vkCreateSemaphore(lDevice, &semaphoreCI, nullptr, &m_ImageSemaphores[i]);
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

        vkDestroySemaphore(lDevice, m_ImageSemaphores[i], nullptr);
    }

    m_ImageSemaphores.clear();
    p_SwapChainImages.clear();

    vkDestroySwapchainKHR(lDevice, m_SwapChain, nullptr);
}

uint32_t VKSwapChain::GetNextSwapChainImage(IDevice* _device, uint32_t _frameIndex)
{
    const VkDevice lDevice = _device->CastToVK()->GetLogicalDevice();

    uint32_t imageIndex = 0;
    vkAcquireNextImageKHR(lDevice, m_SwapChain, UINT64_MAX, m_ImageSemaphores[_frameIndex], VK_NULL_HANDLE, &imageIndex);
    return imageIndex;
}
