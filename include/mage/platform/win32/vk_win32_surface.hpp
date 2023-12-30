//
// Created by Jesse on 12/30/2023.
//

#pragma once

#include <vulkan/vulkan.h>
#include <windows.h>

namespace mage {
    class win32_window;
    class vk_instance;

    class vk_win32_surface final {
        std::reference_wrapper<vk_instance> instance_;
        VkSurfaceKHR surface_;

    public:
        explicit vk_win32_surface(vk_instance& instance,
                                  const win32_window& window,
                                  HINSTANCE hinstance = GetModuleHandle(nullptr));

        vk_win32_surface(const vk_win32_surface& other) noexcept = delete;

        vk_win32_surface(vk_win32_surface&& other) noexcept
            : instance_(other.instance_), surface_(other.surface_) {
            other.surface_ = nullptr;
        }

        ~vk_win32_surface() noexcept;

        vk_win32_surface& operator=(const vk_win32_surface& other) noexcept = delete;

        vk_win32_surface& operator=( vk_win32_surface&& other) noexcept;

        [[nodiscard]]
        auto get() const noexcept -> VkSurfaceKHR {
            return surface_;
        }
    };
}
