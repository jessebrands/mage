//
// Created by Jesse on 12/30/2023.
//

#pragma once

#include <stdexcept>
#include <mage/renderer/vulkan/vk_device.hpp>

mage::vk_device::vk_device(const vk_physical_device& physical_device) : device_(VK_NULL_HANDLE) {
    const auto indices = physical_device.get_queue_families();
    float queue_priorities = 1.0f;

    VkDeviceQueueCreateInfo queue_create_info = {
        .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .queueFamilyIndex = indices.graphics_family.value(),
        .queueCount = 1,
        .pQueuePriorities = &queue_priorities,
    };

    // Select what device features we are interested in using.
    VkPhysicalDeviceFeatures device_features = {};

    // Select what device extensions we are interested in using.
    std::vector enabled_extensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };

    VkDeviceCreateInfo create_info = {
        .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .queueCreateInfoCount = 1,
        .pQueueCreateInfos = &queue_create_info,

        // Device layers are ignored by modern Vulkan specifications.
        .enabledLayerCount = 0,
        .ppEnabledLayerNames = nullptr,

        .enabledExtensionCount = static_cast<uint32_t>(enabled_extensions.size()),
        .ppEnabledExtensionNames = enabled_extensions.data(),
        .pEnabledFeatures = &device_features
    };

    VkResult result = vkCreateDevice(static_cast<VkPhysicalDevice>(physical_device),
                                     &create_info, nullptr, &device_);

    if (result != VK_SUCCESS) {
        throw std::runtime_error("Could not create logical Vulkan device.");
    }
}
