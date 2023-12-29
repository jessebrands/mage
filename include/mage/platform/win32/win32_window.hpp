//
// Created by Jesse on 12/28/2023.
//

#pragma once

#include <mage/platform/win32/win32_window_class.hpp>

namespace mage {
    /*!
     * \brief Main rendering window.
     */
    class win32_window : win32_window_class<win32_window, TEXT("mage::win32_window")> {
        // Base class must be declared a friend so it can call our window procedure.
        friend win32_window_class;

        HWND handle_;

    protected:
        // ------
        // ------ Win32 message queue handling
        // ------

        /*!
         * \brief Win32 window procedure.
         * \see https://learn.microsoft.com/en-us/windows/win32/api/winuser/nc-winuser-wndproc
         */
        static LRESULT CALLBACK window_proc(HWND window, UINT msg, WPARAM wparam, LPARAM lparam);

        /*!
         * \brief Sent when a window is being destroyed.
         * \see https://learn.microsoft.com/en-us/windows/win32/winmsg/wm-destroy
         */
        LRESULT win32_msg_destroy_(WPARAM wparam, LPARAM lparam);

    public:
        explicit win32_window(HINSTANCE instance = GetModuleHandle(nullptr),
                              int show_cmd = SW_SHOW,
                              LPCTSTR title = TEXT("Mage 3D"));

        ~win32_window();

        win32_window(const win32_window& other) = delete;

        win32_window(win32_window&& other) noexcept;

        win32_window& operator=(const win32_window& other) = delete;

        win32_window& operator=(win32_window&& other) noexcept;

        /*!
         * \brief Sets the specified window's show state.
         * \see https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-showwindow
         */
        void win32_show_window(int show_cmd);

        /*!
         * \brief Changes the text of the specified window's title bar (if it has one).
         * \see https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-setwindowtextw
         */
        void win32_set_window_text(LPCTSTR text);

        /*!
         * \brief Returns the handle to the window.
         * \return Win32 handle (HWND) to the window.
         */
        [[nodiscard]]
        auto win32_handle() noexcept -> HWND {
            return handle_;
        }

        /*!
         * \brief Returns the handle to the window.
         * \return Win32 handle (HWND) to the window.
         */
        [[nodiscard]]
        auto win32_handle() const noexcept -> HWND {
            return handle_;
        }

        /*!
         * \brief Operator to explicitly cast to a Win32 window handle.
         */
        explicit operator HWND() const noexcept {
            return handle_;
        }
    };
}
