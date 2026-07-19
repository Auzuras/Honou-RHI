"%VULKAN_SDK%\Bin\glslc.exe" HelloTriangle/HelloTriangle.vert -o HelloTriangle/HelloTriangle.vert.spv
"%VULKAN_SDK%\Bin\glslc.exe" HelloTriangle/HelloTriangle.frag -o HelloTriangle/HelloTriangle.frag.spv
"%VULKAN_SDK%\Bin\glslc.exe" HelloTriangle/HelloTriangle.comp -o HelloTriangle/HelloTriangle.comp.spv

"%VULKAN_SDK%\Bin\glslc.exe" Unlit/Unlit.vert -o Unlit/Unlit.vert.spv
"%VULKAN_SDK%\Bin\glslc.exe" Unlit/Unlit.frag -o Unlit/Unlit.frag.spv
pause