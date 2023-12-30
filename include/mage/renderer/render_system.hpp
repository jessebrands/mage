//
// Created by Jesse on 12/30/2023.
//

#pragma once

#include <algorithm>
#include <map>

#include "vulkan/vk_physical_device.hpp"

namespace mage {
    class render_system final {
    public:
        static std::map<vk_physical_device, int>
        rate_device_suitability(const std::vector<vk_physical_device>& devices) {
            std::map<vk_physical_device, int> rated_devices;

            for (const auto& device: devices) {
                int score = 0;
                const auto properties = device.get_properties();

                switch (properties.deviceType) {
                    case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU: {
                        score += 50000;
                        break;
                    }
                    case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU: {
                        score += 10000;
                        break;
                    }
                    default: {
                        score += 0;
                    }
                }

                rated_devices.emplace(device, score);
            }

            return rated_devices;
        }
    };
}
