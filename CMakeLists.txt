cmake_minimum_required(VERSION 3.20)

project(HONOU_RENDER LANGUAGES CXX)

option(HONOU_RENDER_TEST_OPT "Build Honou Render tests" ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Vulkan REQUIRED)

file(GLOB_RECURSE HONOU_RENDER_SOURCES "Source/*.cpp")

add_library(HonouRender STATIC ${HONOU_RENDER_SOURCES})

target_include_directories(HonouRender PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Include ${Vulkan_INCLUDE_DIRS})

target_link_libraries(HonouRender PRIVATE ${Vulkan_LIBRARIES})

target_link_libraries(HonouRender PRIVATE d3d12 dxgi dxguid d3dcompiler)

target_link_libraries(HonouRender PUBLIC Vulkan::Vulkan)

if(HONOU_RENDER_TEST_OPT)
    enable_testing()

    add_subdirectory(Tests)
endif()