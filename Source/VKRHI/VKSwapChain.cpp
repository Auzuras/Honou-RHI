#include <VKRHI/VKSwapChain.h>

#include <VKRHI/VKInstance.h>
#include <VKRHI/VKDevice.h>

#define VK_USE_PLATFORM_WIN32_KHR
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <vulkan/vulkan_win32.h>

void VKSwapChain::CreateSwapChain(IInstance* _instance, IDevice* _device)
{
    const VkInstance instance = _instance->CastToVK()->GetRaw();
    const VkPhysicalDevice pDevice = _device->CastToVK()->GetPhysicalDevice();
    const VkDevice lDevice = _device->CastToVK()->GetLogicalDevice();

    const VkWin32SurfaceCreateInfoKHR surfaceCI{
        .sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,
        .hinstance = GetModuleHandle(nullptr),
        .hwnd = nullptr
    };

    VkSurfaceKHR surface{};
    vkCreateWin32SurfaceKHR(instance, &surfaceCI, nullptr, &surface);

    VkSurfaceCapabilitiesKHR surfaceCaps{};
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(pDevice, surface, &surfaceCaps);

    VkExtent2D scExtent{ surfaceCaps.currentExtent };
    if (surfaceCaps.currentExtent.width == 0xFFFFFFFF)
        return;

    const VkFormat imageFormat{ VK_FORMAT_B8G8R8A8_UNORM };
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
}

void VKSwapChain::DestroySwapChain(IDevice* _device)
{
    const VkDevice lDevice = _device->CastToVK()->GetLogicalDevice();
    vkDestroySwapchainKHR(lDevice, m_SwapChain, nullptr);
}
