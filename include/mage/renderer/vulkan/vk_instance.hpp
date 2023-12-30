//
// Created by Jesse on 12/30/2023.
//

#pragma once

#include <vector>

#include <vulkan/vulkan.h>

#include <mage/renderer/vulkan/vk_physical_device.hpp>

namespace mage {
    class vk_instance final {
        VkInstance instance_{VK_NULL_HANDLE};

    public:
        vk_instance();

        vk_instance(const vk_instance& other) = delete;

        vk_instance(vk_instance&& other) noexcept : instance_(other.instance_){
            other.instance_ = VK_NULL_HANDLE;
        }

        ~vk_instance();

        [[nodiscard]]
        std::vector<vk_physical_device> enumerate_physical_devices() const;

        vk_instance& operator=(const vk_instance& other) = delete;

        vk_instance& operator=(vk_instance&& other) noexcept {
            if (this != &other) {
                if (instance_ != VK_NULL_HANDLE) {
                    vkDestroyInstance(instance_, nullptr);
                }
                instance_ = other.instance_;
                other.instance_= nullptr;
            }
            return *this;
        }

        explicit operator VkInstance() const noexcept {
            return instance_;
        }

        [[nodiscard]]
        auto get() const noexcept -> VkInstance {
            return instance_;
        }
    };
}
