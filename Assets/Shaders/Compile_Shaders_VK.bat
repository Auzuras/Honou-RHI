"%VULKAN_SDK%\Bin\glslc.exe" HelloTriangle/Vulkan/HelloTriangle.vert -o HelloTriangle/Vulkan/HelloTriangle.vert.spv
"%VULKAN_SDK%\Bin\glslc.exe" HelloTriangle/Vulkan/HelloTriangle.frag -o HelloTriangle/Vulkan/HelloTriangle.frag.spv
"%VULKAN_SDK%\Bin\glslc.exe" HelloTriangle/Vulkan/HelloTriangle.comp -o HelloTriangle/Vulkan/HelloTriangle.comp.spv

"%VULKAN_SDK%\Bin\glslc.exe" Unlit/Vulkan/Unlit.vert -o Unlit/Vulkan/Unlit.vert.spv
"%VULKAN_SDK%\Bin\glslc.exe" Unlit/Vulkan/Unlit.frag -o Unlit/Vulkan/Unlit.frag.spv
pause