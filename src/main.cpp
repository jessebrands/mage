#include <iostream>
#include <windows.h>

#include <mage/platform/win32/win32_window.hpp>
#include <mage/renderer/vulkan/vk_instance.hpp>

int APIENTRY WinMain([[maybe_unused]] HINSTANCE instance,
                     [[maybe_unused]] HINSTANCE prev_instance,
                     [[maybe_unused]] PSTR cmd_line,
                     [[maybe_unused]] int show_cmd) {
    mage::win32_window window(instance, show_cmd);
    mage::vk_instance inst;
    auto devices = inst.enumerate_physical_devices();

    for (const auto& device : devices) {
        const auto properties = device.get_properties();
        const auto features = device.get_features();
        const auto queue_family_properties = device.get_queue_family_properties();
        std::cout << properties.deviceName << std::endl;
    }

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
