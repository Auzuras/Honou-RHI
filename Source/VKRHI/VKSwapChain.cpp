#include <VKRHI/VKSwapChain.h>

#include <VKRHI/VKDevice.h>

void VKSwapChain::CreateSwapChain(IDevice* _device)
{
    const VkPhysicalDevice pDevice = _device->CastToVK()->GetPhysicalDevice();
    const VkDevice lDevice = _device->CastToVK()->GetLogicalDevice();

    VkSurfaceKHR surface{};

    //VkWin32SurfaceCreateInfoKHR createInfo{
    //    .sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    //    .hwnd = glfwGetWin32Window(window);
    //    .hinstance = GetModuleHandle(nullptr);
    //
    //};
    //vkCreateWin32SurfaceKHR()

    VkSurfaceCapabilitiesKHR surfaceCaps{};
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(pDevice, nullptr, &surfaceCaps);

    const VkFormat imageFormat{ VK_FORMAT_B8G8R8A8_UNORM };
    VkSwapchainCreateInfoKHR swapChainCI{
        .sType              =   VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
        .surface            =   surface,
        .minImageCount      =   surfaceCaps.minImageCount,
        .imageFormat        =   imageFormat,
        .imageColorSpace    =   VK_COLOR_SPACE_SRGB_NONLINEAR_KHR,
        .imageExtent{
            .width          =   surfaceCaps.currentExtent.width,
            .height         =   surfaceCaps.currentExtent.height,
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
