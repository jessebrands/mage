cmake_minimum_required(VERSION 3.27)

project(mage
        VERSION 0.1.0
        DESCRIPTION "Toy graphics renderer."
        HOMEPAGE_URL "https://github.com/jessebrands/mage"
        LANGUAGES CXX)

###
### Mage3D only supports Win32/D3D11 platform.
if (NOT WIN32)
    message(FATAL_ERROR "Mage only supports building on Win32 platforms.")
endif ()

###
### Source lists.
set(MAGE_SOURCES
        src/main.cpp)

###
### Platform abstraction source lists.
set(MAGE_PLATFORM_HEADERS
        include/mage/platform/win32/win32_window_class.hpp
        include/mage/platform/win32/win32_window.hpp)

set(MAGE_PLATFORM_SOURCES
        src/platform/win32/win32_window.cpp)

set(MAGE_RENDERER_SOURCES)

add_executable(mage
        ${MAGE_SOURCES}
        ${MAGE_PLATFORM_HEADERS}
        ${MAGE_PLATFORM_SOURCES}
        ${MAGE_RENDERER_HEADERS}
        ${MAGE_RENDERER_SOURCES})

###
### We want the most up-to-date standard and no extensions.
set_target_properties(mage PROPERTIES
        CXX_STANDARD 23
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
        WIN32_EXECUTABLE ON
        COMPILE_WARNING_AS_ERROR ON)

target_include_directories(mage
        PRIVATE ${PROJECT_SOURCE_DIR}/include)

###
### Win32 L&M may speed builds up slightly. Unicode clarifies that we want UTF-16 support.
target_compile_definitions(mage
        PUBLIC WIN32_LEAN_AND_MEAN=1
        PUBLIC UNICODE=1
        PRIVATE MAGE_VERSION="${PROJECT_VERSION}"
        PRIVATE MAGE_PLATFORM_WIN32=1
        PRIVATE MAGE_PLATFORM="Win32")

###
### Set up libraries
find_package(Vulkan REQUIRED)
target_link_libraries(mage
        PRIVATE Vulkan::Vulkan)

###
### Compiler specific settings.
if (MSVC)
    target_compile_options(mage
            PRIVATE /W4)
else ()
    target_compile_options(mage
            PRIVATE -Wall -Wextra -Wpedantic)
endif ()
