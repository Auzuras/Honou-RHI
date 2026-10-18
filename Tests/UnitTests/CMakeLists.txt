include(FetchContent)

set(BUILD_GMOCK OFF)
set(INSTALL_GTEST OFF)
set(gtest_disable_pthreads ON)
set(KOORI_WINDOW_TEST_OPT OFF)
set(KOORI_MATH_TEST_OPT OFF)

FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.17.0
        SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/googletest
)

FetchContent_Declare(
        tinyobjloader
        GIT_REPOSITORY https://github.com/tinyobjloader/tinyobjloader.git
        GIT_TAG release
        SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/tinyobjloader
)

FetchContent_Declare(
        stb
        GIT_REPOSITORY https://github.com/nothings/stb.git
        GIT_TAG master
        SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/stb
)

FetchContent_Declare(
        Koori-Math
        GIT_REPOSITORY https://github.com/Auzuras/Koori-Math.git 
        GIT_TAG dev
        SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/Koori-Math
)

FetchContent_Declare(
        Koori-Window
        GIT_REPOSITORY https://github.com/Auzuras/Koori-Window.git 
        GIT_TAG dev
        SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/Koori-Window
)

FetchContent_MakeAvailable(googletest tinyobjloader stb Koori-Math Koori-Window)

set(HONOU_RHI_UT_BIN_OUTPUT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Build/${CMAKE_CXX_COMPILER_ID}_$<CONFIG>")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${HONOU_RHI_UT_BIN_OUTPUT_DIR}/Libs")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${HONOU_RHI_UT_BIN_OUTPUT_DIR}/Libs")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${HONOU_RHI_UT_BIN_OUTPUT_DIR}")

file(GLOB_RECURSE HONOU_RHI_UT_SOURCES "Code/Source/*.cpp")

add_executable(HonouRHITests ${HONOU_RHI_UT_SOURCES})

add_custom_command(TARGET HonouRHITests POST_BUILD
COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/Assets
            $<TARGET_FILE_DIR:HonouRHITests>/Assets
)

target_include_directories(HonouRHITests PRIVATE ${stb_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/Code/Include)

target_link_libraries(HonouRHITests PRIVATE gtest)

target_link_libraries(HonouRHITests PRIVATE tinyobjloader)

target_link_libraries(HonouRHITests PRIVATE KooriMath)

target_link_libraries(HonouRHITests PRIVATE KooriWindow)

target_link_libraries(HonouRHITests PRIVATE HonouRHI)

if (MSVC)
    target_compile_options(HonouRHITests PRIVATE /W4 /WX)
else ()
    target_compile_options(HonouRHITests PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif ()
