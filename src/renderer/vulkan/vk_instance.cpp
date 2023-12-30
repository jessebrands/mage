//
// Created by Jesse on 12/30/2023.
//

#include <stdexcept>
#include <vector>

#include <mage/renderer/vulkan/vk_instance.hpp>

mage::vk_instance::vk_instance() {
    VkApplicationInfo info = {
        .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .pNext = nullptr,
        .pApplicationName = "Mage 3D",
        .applicationVersion = VK_MAKE_VERSION(0, 1, 0),
        .pEngineName = "Mage",
        .engineVersion = VK_MAKE_VERSION(0, 1, 0),
        .apiVersion = VK_API_VERSION_1_1
    };

    const std::vector enabled_layers = {
        "VK_LAYER_KHRONOS_validation",
    };

    const std::vector enabled_extensions = {
        VK_KHR_SURFACE_EXTENSION_NAME,
        VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
    };

    const VkInstanceCreateInfo create_info = {
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .pApplicationInfo = &info,
        .enabledLayerCount = static_cast<uint32_t>(enabled_layers.size()),
        .ppEnabledLayerNames = enabled_layers.data(),
        .enabledExtensionCount = static_cast<uint32_t>(enabled_extensions.size()),
        .ppEnabledExtensionNames = enabled_extensions.data(),
    };

    VkResult result = vkCreateInstance(&create_info, nullptr, &instance_);
    if (result != VK_SUCCESS) {
        throw std::runtime_error("failed to create Vulkan instance");
    }
}

mage::vk_instance::~vk_instance() {
    vkDestroyInstance(instance_, nullptr);
}

std::vector<mage::vk_physical_device> mage::vk_instance::enumerate_physical_devices() const {
    uint32_t device_count = 0;
    vkEnumeratePhysicalDevices(instance_, &device_count, nullptr);
    if (device_count == 0) {
        throw std::runtime_error("no devices supporting Vulkan found");
    }

    // Now we actually load up a vector with the device handles.
    std::vector<VkPhysicalDevice> devices(device_count);
    VkResult result = vkEnumeratePhysicalDevices(instance_, &device_count, devices.data());
    if (result != VK_SUCCESS) {
        throw std::runtime_error("failed to enumerate devices");
    }

    std::vector<vk_physical_device> physical_devices;
    for (auto device : devices) {
        physical_devices.emplace_back(device);
    }

    return physical_devices;
}
