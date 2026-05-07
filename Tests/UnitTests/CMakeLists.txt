include(FetchContent)

set(BUILD_GMOCK OFF)
set(INSTALL_GTEST OFF)
set(gtest_disable_pthreads ON)
set(KOORI_WINDOW_TEST_OPT OFF)

FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.17.0
        SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/googletest
)

FetchContent_MakeAvailable(googletest)

add_subdirectory(ThirdParty/Koori-Window)

set(HONOU_RENDER_UT_BIN_OUTPUT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Build/${CMAKE_CXX_COMPILER_ID}_$<CONFIG>")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${HONOU_RENDER_UT_BIN_OUTPUT_DIR}/Libs")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${HONOU_RENDER_UT_BIN_OUTPUT_DIR}/Libs")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${HONOU_RENDER_UT_BIN_OUTPUT_DIR}")

file(GLOB_RECURSE HONOU_RENDER_UT_SOURCES "Code/Source/*.cpp")

add_executable(HonouRenderTests ${HONOU_RENDER_UT_SOURCES})

target_link_libraries(HonouRenderTests PRIVATE gtest)

target_link_libraries(HonouRenderTests PRIVATE KooriWindow)

target_link_libraries(HonouRenderTests PRIVATE HonouRender)