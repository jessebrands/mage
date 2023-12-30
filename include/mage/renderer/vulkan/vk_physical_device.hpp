//
// Created by Jesse on 12/30/2023.
//

#pragma once

#include <optional>
#include <vector>
#include <vulkan/vulkan.h>

namespace mage {
    struct vk_queue_family_indices final {
        std::optional<uint32_t> graphics_family;
    };

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

        bool operator<(const vk_physical_device& other) const noexcept {
            return device_ < other.device_;
        }

        explicit operator VkPhysicalDevice() const noexcept {
            return device_;
        }

        [[nodiscard]]
        VkPhysicalDeviceProperties get_properties() const noexcept;

        [[nodiscard]]
        VkPhysicalDeviceFeatures get_features() const noexcept;

        [[nodiscard]]
        std::vector<VkQueueFamilyProperties> get_queue_family_properties() const noexcept;

        [[nodiscard]]
        vk_queue_family_indices get_queue_families() const noexcept;
    };
}
