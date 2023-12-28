//
// Created by Jesse on 12/28/2023.
//

#include <mage/platform/win32/win32_window.hpp>

LRESULT mage::win32_window::window_proc(HWND window, UINT msg, WPARAM wparam, LPARAM lparam) {
    if (msg == WM_NCCREATE) {
        const auto create_struct = reinterpret_cast<LPCREATESTRUCT>(lparam);
        auto self = static_cast<win32_window *>(create_struct->lpCreateParams);
        SetWindowLongPtr(window, 0, reinterpret_cast<LONG_PTR>(self));
        return TRUE;
    }

    auto self = reinterpret_cast<win32_window *>(GetWindowLongPtr(window, 0));
    if (self == nullptr) {
        return DefWindowProc(window, msg, wparam, lparam);
    }

    return DefWindowProc(window, msg, wparam, lparam);
}

mage::win32_window::win32_window(HINSTANCE instance, int show_cmd, LPCSTR title) : win32_window_class(instance) {
    handle_ = CreateWindowExA(
        WS_EX_APPWINDOW | WS_EX_OVERLAPPEDWINDOW,
        win32_window_class_name(),
        title,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        CW_USEDEFAULT, CW_USEDEFAULT,
        nullptr,
        nullptr,
        instance,
        this
    );

    if (handle_ == nullptr) {
        throw std::runtime_error("could not create win32 window");
    }

    win32_show_window(show_cmd);
}

mage::win32_window::~win32_window() {
    if (handle_ != nullptr) {
        DestroyWindow(handle_);
    }
}

void mage::win32_window::win32_show_window(int show_cmd) {
    ShowWindow(handle_, show_cmd);
}
