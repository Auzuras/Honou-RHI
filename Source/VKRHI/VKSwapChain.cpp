#include <VKRHI/VKSwapChain.h>

#include <VKRHI/VKInstance.h>
#include <VKRHI/VKDevice.h>
#include <VKRHI/VKImage.h>
#include <VKRHI/VKQueue.h>
#include <VKRHI/VKFence.h>

#include <string>

#define VK_USE_PLATFORM_WIN32_KHR
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <vulkan/vulkan_win32.h>

void VKSwapChain::CreateSwapChain(IInstance* _instance, IDevice* _device, const Description& _description)
{
    const VkInstance instance = _instance->CastToVK()->GetRaw();
    const VkPhysicalDevice pDevice = _device->CastToVK()->GetPhysicalDevice();
    const VkDevice lDevice = _device->CastToVK()->GetLogicalDevice();

    const VkWin32SurfaceCreateInfoKHR surfaceCI{
        .sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,
        .hinstance = _description.instance,
        .hwnd = _description.hwnd
    };

    vkCreateWin32SurfaceKHR(instance, &surfaceCI, nullptr, &m_Surface);

    const VkSwapchainCreateInfoKHR swapChainCI = CreateSwapChainInfo(lDevice, pDevice, m_Surface, _description.width, _description.height);

    vkCreateSwapchainKHR(lDevice, &swapChainCI, nullptr, &m_SwapChain);

    CreateSwapChainImages(_device, _description.width, _description.height);

    CreateSwapChainSyncObjects(_device);
}

void VKSwapChain::RecreateSwapChain(IDevice* _device, const Description& _description)
{
    const VkSurfaceKHR surface = m_Surface;
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

void VKSwapChain::DestroySwapChain(IInstance* _instance, IDevice* _device)
{
    const VkInstance instance = _instance->CastToVK()->GetRaw();
    const VkDevice lDevice = _device->CastToVK()->GetLogicalDevice();

    DestroySwapChainImages(_device);

    DestroySwapChainSyncObjects(_device);

    vkDestroySwapchainKHR(lDevice, m_SwapChain, nullptr);

    vkDestroySurfaceKHR(instance, m_Surface, nullptr);
}

uint32_t VKSwapChain::Acquire(IDevice* _device, uint32_t _frameIndex)
{
    const VkDevice lDevice = _device->CastToVK()->GetLogicalDevice();

    const uint64_t completedValue = p_SwapChainFence->GetCompletedValue(_device);
    
    if (completedValue < p_FenceValues[_frameIndex])
    {
        p_SwapChainFence->WaitCPU(_device, p_FenceValues[_frameIndex]);
    }

    VkResult result = vkAcquireNextImageKHR(lDevice, m_SwapChain, UINT64_MAX, m_ImageSemaphores[_frameIndex], VK_NULL_HANDLE, &p_CurrentImageIndex);

    if (result != VK_SUCCESS)
    {
        result = result;
    }

    return p_CurrentImageIndex;
}

void VKSwapChain::Present(IDevice* _device, IQueue* _presentQueue, uint32_t _frameIndex)
{
    const VkDevice lDevice = _device->CastToVK()->GetLogicalDevice();
    const VkQueue queue = _presentQueue->CastToVK()->GetRaw();

    VkPipelineStageFlags waitStages = VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT;
    
    const uint64_t signalValue = ++p_NextFenceValue;
    p_FenceValues[_frameIndex] = signalValue;

    const VkPipelineStageFlags2 waitStage = VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT;

    const VkSemaphoreSubmitInfo waitInfo{
        .sType = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO,
        .semaphore = m_ImageSemaphores[_frameIndex],
        .value = 0,
        .stageMask = waitStage
    };

    const VkSemaphoreSubmitInfo signalInfos[]{
        {
            .sType = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO,
            .semaphore = m_RenderFinishedSemaphores[p_CurrentImageIndex],
            .value = 0,
            .stageMask = VK_PIPELINE_STAGE_2_ALL_COMMANDS_BIT
        },
        {
            .sType = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO,
            .semaphore = p_SwapChainFence->CastToVK()->GetRaw(),
            .value = signalValue,
            .stageMask = VK_PIPELINE_STAGE_2_ALL_COMMANDS_BIT
        }
    };

    // This submit is used here to hide 
    // the internal synchronization of the swapchain unique to vulkan
    const VkSubmitInfo2 submitInfo{
        .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO_2,
        .waitSemaphoreInfoCount = 1,
        .pWaitSemaphoreInfos = &waitInfo,
        .signalSemaphoreInfoCount = 2,
        .pSignalSemaphoreInfos = signalInfos
    };

    vkQueueSubmit2(queue, 1, &submitInfo, VK_NULL_HANDLE);

    VkPresentInfoKHR presentInfo{
        .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
        .waitSemaphoreCount = 1,
        .pWaitSemaphores = &m_RenderFinishedSemaphores[p_CurrentImageIndex],
        .swapchainCount = 1,
        .pSwapchains = &m_SwapChain,
        .pImageIndices = &p_CurrentImageIndex
    };

    VkResult result = vkQueuePresentKHR(queue, &presentInfo);

    if (result != VK_SUCCESS)
    {
        result = result;
    }
}

void VKSwapChain::CreateSwapChainImages(IDevice* _device, uint32_t _width, uint32_t _height)
{
    const VkDevice lDevice = _device->CastToVK()->GetLogicalDevice();

    vkGetSwapchainImagesKHR(lDevice, m_SwapChain, &p_SwapChainImagesCount, nullptr);
    std::vector<VkImage> scImages = std::vector<VkImage>(p_SwapChainImagesCount);
    vkGetSwapchainImagesKHR(lDevice, m_SwapChain, &p_SwapChainImagesCount, scImages.data());

    const IImage::Description desc{
        .width = _width,
        .height = _height,
        .format = IImage::Format::HONOU_FORMAT_B8G8R8A8_UNORM
    };

    // TODO: Rework
    for (size_t i = 0; i < p_SwapChainImagesCount; ++i)
    {
        VKImage* image = new VKImage();
        image->CreateSwapChainImage(_device, scImages[i], desc);
        std::string name = "Hounou RHI - Swap Chain Image " + std::to_string(i);
        image->SetName(_device, name.c_str());
        p_SwapChainImages.push_back(image);
    }
}

void VKSwapChain::DestroySwapChainImages(IDevice* _device)
{
    for (size_t i = 0; i < p_SwapChainImagesCount; ++i)
    {
        p_SwapChainImages[i]->CastToVK()->DestroySwapChainImage(_device);
        delete p_SwapChainImages[i];
    }

    p_SwapChainImages.clear();
}

void VKSwapChain::CreateSwapChainSyncObjects(IDevice* _device)
{
    VkDevice const lDevice = _device->CastToVK()->GetLogicalDevice();

    const VkSemaphoreCreateInfo semaphoreCI{
        .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO
    };

    for (size_t i = 0; i < ISwapChain::MAX_FRAMES_IN_FLIGHT; ++i)
    {
        vkCreateSemaphore(lDevice, &semaphoreCI, VK_NULL_HANDLE, &m_ImageSemaphores[i]);
    }

    m_RenderFinishedSemaphores.resize(p_SwapChainImagesCount);

    for (size_t i = 0; i < p_SwapChainImagesCount; ++i)
    {
        vkCreateSemaphore(lDevice, &semaphoreCI, VK_NULL_HANDLE, &m_RenderFinishedSemaphores[i]);
    }

    p_SwapChainFence = new VKFence();
    p_SwapChainFence->CreateFence(_device);
}

void VKSwapChain::DestroySwapChainSyncObjects(IDevice* _device)
{
    VkDevice const lDevice = _device->CastToVK()->GetLogicalDevice();

    for (size_t i = 0; i < ISwapChain::MAX_FRAMES_IN_FLIGHT; ++i)
    {
        vkDestroySemaphore(lDevice, m_ImageSemaphores[i], VK_NULL_HANDLE);
    }

    for (size_t i = 0; i < p_SwapChainImagesCount; ++i)
    {
        vkDestroySemaphore(lDevice, m_RenderFinishedSemaphores[i], VK_NULL_HANDLE);
    }

    m_RenderFinishedSemaphores.clear();

    p_SwapChainFence->DestroyFence(_device);
    delete p_SwapChainFence;
}

VkSwapchainCreateInfoKHR VKSwapChain::CreateSwapChainInfo(VkDevice _device, VkPhysicalDevice _physicalDevice, VkSurfaceKHR _surface, uint32_t _width, uint32_t _height)
{
    VkSurfaceCapabilitiesKHR surfaceCaps{};
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(_physicalDevice, _surface, &surfaceCaps);

    VkExtent2D scExtent{ surfaceCaps.currentExtent };
    if (surfaceCaps.currentExtent.width == 0xFFFFFFFF)
        return VkSwapchainCreateInfoKHR();

    const IImage::Format scFormat = IImage::Format::HONOU_FORMAT_B8G8R8A8_UNORM;
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
