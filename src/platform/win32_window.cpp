//
// Created by Jesse on 12/28/2023.
//

#include <mage/platform/win32_window.hpp>

LRESULT mage::win32_window::window_proc(HWND window, UINT msg, WPARAM wparam, LPARAM lparam) {
    if (msg == WM_NCCREATE) {
        const auto create_struct = reinterpret_cast<LPCREATESTRUCT>(lparam);
        auto self = static_cast<win32_window *>(create_struct->lpCreateParams);
        SetWindowLongPtr(window, 0, reinterpret_cast<LONG_PTR>(self));
        // The documentation suggests returning 0, but doing that breaks the default window title.
        // Proceed with calling the DefaultWindowProc instead.
        return DefWindowProc(window, msg, wparam, lparam);
    }

    auto self = reinterpret_cast<win32_window *>(GetWindowLongPtr(window, 0));
    if (self != nullptr) {
        switch (msg) {
            case WM_DESTROY: return self->win32_msg_destroy_(wparam, lparam);
            default: return DefWindowProc(window, msg, wparam, lparam);
        }
    }

    return DefWindowProc(window, msg, wparam, lparam);
}

LRESULT mage::win32_window::win32_msg_destroy_([[maybe_unused]] WPARAM wparam, [[maybe_unused]] LPARAM lparam) {
    PostQuitMessage(0);
    return 0;
}

mage::win32_window::win32_window(HINSTANCE instance, int show_cmd, LPCTSTR title) : win32_window_class(instance) {
    handle_ = CreateWindowEx(
        0,
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

mage::win32_window::win32_window(win32_window&& other) noexcept : handle_(other.handle_) {
    other.handle_ = nullptr;

    // To not break the window_proc, we must remap the window class extra bytes memory to this.
    if (handle_ != nullptr) {
        SetWindowLongPtr(handle_, 0, reinterpret_cast<LONG_PTR>(this));
    }
}

mage::win32_window& mage::win32_window::operator=(win32_window&& other) noexcept {
    if (this != &other) {
        if (handle_ != nullptr) {
            DestroyWindow(handle_);
        }
        handle_ = other.handle_;
        other.handle_ = nullptr;
        if (handle_ != nullptr) {
            SetWindowLongPtr(handle_, 0, reinterpret_cast<LONG_PTR>(this));
        }
    }
    return *this;
}


void mage::win32_window::win32_show_window(int show_cmd) {
    ShowWindow(handle_, show_cmd);
}

void mage::win32_window::win32_set_window_text(LPCTSTR text) {
    SetWindowText(handle_, text);
}
