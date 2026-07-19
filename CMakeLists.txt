cmake_minimum_required(VERSION 3.20)

project(HONOU_RHI LANGUAGES CXX)

option(HONOU_RHI_TEST_OPT "Build Honou RHI tests" ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Vulkan REQUIRED)

file(GLOB_RECURSE HONOU_RHI_SOURCES "Source/*.cpp")

add_library(HonouRHI STATIC ${HONOU_RHI_SOURCES})

target_include_directories(HonouRHI PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Include ${Vulkan_INCLUDE_DIRS})

target_link_libraries(HonouRHI PRIVATE ${Vulkan_LIBRARIES})

target_link_libraries(HonouRHI PRIVATE d3d12 dxgi dxguid d3dcompiler)

target_link_libraries(HonouRHI PUBLIC Vulkan::Vulkan)

if(HONOU_RHI_TEST_OPT)
    enable_testing()

    add_subdirectory(Tests)
endif()