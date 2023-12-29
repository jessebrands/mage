#include <iostream>
#include <windows.h>

#include <mage/platform/win32/win32_window.hpp>

int APIENTRY WinMain([[maybe_unused]] HINSTANCE instance,
                     [[maybe_unused]] HINSTANCE prev_instance,
                     [[maybe_unused]] PSTR cmd_line,
                     [[maybe_unused]] int show_cmd) {
    mage::win32_window window(instance, show_cmd);

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
