#include <iostream>
#include <windows.h>

#include <mage/platform/win32/win32_window.hpp>
#include <mage/renderer/vulkan/vk_instance.hpp>

#include <mage/renderer/render_system.hpp>

#include "mage/platform/win32/vk_win32_surface.hpp"
#include "mage/renderer/vulkan/vk_device.hpp"

int APIENTRY WinMain([[maybe_unused]] HINSTANCE hinstance,
                     [[maybe_unused]] HINSTANCE prev_instance,
                     [[maybe_unused]] PSTR cmd_line,
                     [[maybe_unused]] int show_cmd) {
    mage::win32_window window(hinstance, show_cmd);
    mage::vk_instance instance;
    mage::vk_surface surface(instance, window, hinstance);
    const auto devices = instance.enumerate_physical_devices();
    const auto selected_device = devices[0];
    const auto queue_indices = selected_device.get_queue_families(surface);
    mage::vk_device device(selected_device, queue_indices);

    MSG msg;
    BOOL ret;
    while ((ret = GetMessage(&msg, nullptr, 0, 0)) != 0) {
        if (ret == -1) {
            return ret;
        }
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return ret;
}
