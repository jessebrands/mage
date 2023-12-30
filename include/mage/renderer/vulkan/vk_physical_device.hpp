//
// Created by Jesse on 12/30/2023.
//

#pragma once

#include <vector>
#include <vulkan/vulkan.h>

namespace mage {
    class vk_physical_device final {
        VkPhysicalDevice device_{VK_NULL_HANDLE};

    public:
        vk_physical_device() = default;

        explicit vk_physical_device(VkPhysicalDevice device) : device_(device) {
            // Intentionally left blank.
        }

        vk_physical_device(const vk_physical_device& other) noexcept = default;
        vk_physical_device(vk_physical_device&& other) noexcept = default;

        vk_physical_device& operator=(const vk_physical_device& other) noexcept = default;
        vk_physical_device& operator=(vk_physical_device&& other) noexcept = default;

        explicit operator VkPhysicalDevice() const noexcept {
            return device_;
        }

        [[nodiscard]]
        VkPhysicalDeviceProperties get_properties() const noexcept;

        [[nodiscard]]
        VkPhysicalDeviceFeatures get_features() const noexcept;

        [[nodiscard]]
        std::vector<VkQueueFamilyProperties> get_queue_family_properties() const noexcept;
    };
}
