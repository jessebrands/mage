//
// Created by Jesse on 12/30/2023.
//

#include <mage/renderer/vulkan/vk_instance.hpp>
#include <mage/platform/win32/vk_win32_surface.hpp>
#include <mage/platform/win32/win32_window.hpp>

mage::vk_win32_surface::vk_win32_surface(vk_instance& instance,
                                         const win32_window& window,
                                         HINSTANCE hinstance)
:  instance_(instance), surface_(nullptr) {
    const VkWin32SurfaceCreateInfoKHR create_info = {
        .sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,
        .pNext = nullptr,
        .flags = 0,
        .hinstance = hinstance,
        .hwnd = window.win32_handle(),
    };

    VkResult result = vkCreateWin32SurfaceKHR(instance_.get().get(), &create_info, nullptr, &surface_);
    if (result != VK_SUCCESS) {
        throw std::runtime_error("Could not create Win32 Vulkan surface.");
    }
}

mage::vk_win32_surface::~vk_win32_surface() {
    vkDestroySurfaceKHR(instance_.get().get(), surface_, nullptr);
}

mage::vk_win32_surface& mage::vk_win32_surface::operator=(vk_win32_surface&& other) noexcept {
    if (this != &other) {
        if (surface_ != nullptr) {
            vkDestroySurfaceKHR(instance_.get().get(), surface_, nullptr);
        }
        instance_ = other.instance_;
        surface_ = other.surface_;
        other.surface_ = nullptr;
    }
    return *this;
}
