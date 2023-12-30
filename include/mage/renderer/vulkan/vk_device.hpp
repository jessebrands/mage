//
// Created by Jesse on 12/30/2023.
//

#pragma once

#include <mage/renderer/vulkan/vk_physical_device.hpp>

namespace mage {
    class vk_device final {
        VkDevice device_;

    public:
        explicit vk_device(const vk_physical_device& device);

        vk_device(const vk_device& other) noexcept = delete;

        vk_device(vk_device&& other) noexcept : device_(other.device_) {
            other.device_ = nullptr;
        }

        vk_device& operator=(const vk_device& other) noexcept = delete;

        vk_device& operator=(vk_device&& other) noexcept {
            if (this != &other) {
                if (device_ != nullptr) {
                    vkDestroyDevice(device_, nullptr);
                }
                device_ = other.device_;
                other.device_ = nullptr;
            }
            return *this;
        }

        ~vk_device() {
            vkDestroyDevice(device_, nullptr);
        }
    };
}
