//
// Created by Jesse on 12/28/2023.
//

#pragma once

#include <atomic>
#include <cassert>
#include <stdexcept>
#include <vcruntime_typeinfo.h>
#include <windows.h>

namespace mage {
    template<class T>
    class win32_window_class {
        using type_t = win32_window_class;
        using window_t = T;

        // The implementing class must be declared as a friend.
        friend window_t;

        // Reference counting for appropriate registration and deletion of the class.
        static inline HINSTANCE instance_{nullptr};
        static inline ATOM atom_{0};
        static inline std::atomic_ulong counter_{0};
        static const char* window_class_name_;

        explicit win32_window_class(HINSTANCE instance = GetModuleHandle(nullptr),
                                    UINT style = CS_HREDRAW | CS_VREDRAW,
                                    HICON icon = LoadIcon(nullptr, IDI_APPLICATION),
                                    HCURSOR cursor = LoadCursor(nullptr, IDC_ARROW)) {
            // Check if this class is already registered...
            if (counter_ == 0 && atom_ == 0) {
                instance_ = instance;

                const WNDCLASSEXA wc = {
                    .cbSize = sizeof(WNDCLASSEX),
                    .style = style,
                    .lpfnWndProc = window_t::window_proc,
                    .cbClsExtra = 0,
                    .cbWndExtra = sizeof(type_t *),
                    .hInstance = instance_,
                    .hIcon = icon,
                    .hCursor = cursor,
                    .hbrBackground = reinterpret_cast<HBRUSH>(COLOR_BACKGROUND),
                    .lpszMenuName = nullptr,
                    .lpszClassName = window_class_name_,
                    .hIconSm = nullptr,
                };

                atom_ = RegisterClassExA(&wc);

                if (atom_ == 0) {
                    // TODO: Dedicated Win32 exception class.
                    throw std::runtime_error("could not initialize window class");
                }
            }

            if (instance_ != instance) {
                throw std::runtime_error("win32_window_class invoked with different HINSTANCE");
            }

            // Increase the reference count for this Window Class.
            ++counter_;
        }

        ~win32_window_class() {
            assert(counter_ != 0 && atom_ != 0);

            --counter_;
            if (counter_ == 0 && atom_ != 0) {
                UnregisterClassA(window_class_name_, instance_);
                atom_ = 0;
                instance_ = nullptr;
            }
        }

        [[nodiscard]]
        static constexpr auto win32_window_class_name() noexcept {
            return window_class_name_;
        }

    public:
        win32_window_class([[maybe_unused]] const win32_window_class& other) {
            ++counter_;
        }

        win32_window_class([[maybe_unused]] win32_window_class&& other) noexcept {
            // this might seem unintuitive, but it makes sense!
            // when we move construct, 'other' still exists and will decrement the refcount
            // once it goes out of scope; therefore we must increase the count here.
            ++counter_;
        };

        // Unintuively, the defaults are fine here. We are moving a win32_window_class<T>
        // into another win32_window_class<T> through assignment. The amount of them didn't actually change!
        win32_window_class& operator=(const win32_window_class& other) = default;

        win32_window_class& operator =(win32_window_class&& other) noexcept = default;
    };

    template<typename T>
    const char* win32_window_class<T>::window_class_name_ = typeid(T).name();
}
