#include <VKRHI/VKSurface.h>

#include <VKRHI/VKInstance.h>

#define VK_USE_PLATFORM_WIN32_KHR
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <vulkan/vulkan_win32.h>

void VKSurface::CreateSurface(IInstance* _instance, const SurfaceInformation& _surfaceInfo)
{
    const VkInstance instance = _instance->CastToVK()->GetRaw();

    const VkWin32SurfaceCreateInfoKHR surfaceCI{
        .sType      =   VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,
        .hinstance  =   _surfaceInfo.instance,
        .hwnd       =   _surfaceInfo.hwnd
    };

    vkCreateWin32SurfaceKHR(instance, &surfaceCI, nullptr, &m_Surface);
}

void VKSurface::DestroySurface(IInstance* _instance)
{
    const VkInstance instance = _instance->CastToVK()->GetRaw();
    vkDestroySurfaceKHR(instance, m_Surface, nullptr);
}