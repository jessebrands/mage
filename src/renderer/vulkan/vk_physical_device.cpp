//
// Created by Jesse on 12/30/2023.
//

#include <mage/renderer/vulkan/vk_physical_device.hpp>

VkPhysicalDeviceProperties mage::vk_physical_device::get_properties() const noexcept {
    VkPhysicalDeviceProperties properties;
    vkGetPhysicalDeviceProperties(device_,  &properties);
    return properties;
}

VkPhysicalDeviceFeatures mage::vk_physical_device::get_features() const noexcept {
    VkPhysicalDeviceFeatures features;
    vkGetPhysicalDeviceFeatures(device_, &features);
    return features;
}

std::vector<VkQueueFamilyProperties> mage::vk_physical_device::get_queue_family_properties() const noexcept {
    uint32_t queue_family_count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device_, &queue_family_count, nullptr);

    std::vector<VkQueueFamilyProperties> queue_family_properties(queue_family_count);
    vkGetPhysicalDeviceQueueFamilyProperties(device_, &queue_family_count, queue_family_properties.data());

    return queue_family_properties;
}

mage::vk_queue_family_indices mage::vk_physical_device::get_queue_families() const noexcept {
    vk_queue_family_indices indices;
    const auto family_properties = get_queue_family_properties();

    int index = 0;
    for (const auto& props : family_properties) {
        if (props.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            indices.graphics_family = index;
        }

        ++index;
    }

    return indices;
}
