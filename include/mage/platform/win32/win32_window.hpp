//
// Created by Jesse on 12/28/2023.
//

#pragma once

#include <mage/platform/win32/win32_window_class.hpp>

namespace mage {
    class win32_window : win32_window_class<win32_window> {
        friend win32_window_class;

        HWND handle_ = nullptr;

    protected:
        static LRESULT CALLBACK window_proc(HWND window, UINT msg, WPARAM wparam, LPARAM lparam);

    public:
        explicit win32_window(HINSTANCE instance = GetModuleHandle(nullptr),
                              [[maybe_unused]] int show_cmd = SW_SHOW,
                              LPCSTR title = "Mage");

        ~win32_window();

        void show() {
            win32_show_window(SW_SHOW);
        }

        void win32_show_window(int show_cmd);

        [[nodiscard]]
        auto win32_handle() noexcept -> HWND {
            return handle_;
        }

        [[nodiscard]]
        auto win32_handle() const noexcept -> HWND {
            return handle_;
        }
    };
}
