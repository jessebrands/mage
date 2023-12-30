//
// Created by Jesse on 12/30/2023.
//

#pragma once

#ifdef MAGE_PLATFORM_WIN32
#include <mage/platform/win32/vk_win32_surface.hpp>

namespace mage {
    using vk_surface = vk_win32_surface;
}
#endif
