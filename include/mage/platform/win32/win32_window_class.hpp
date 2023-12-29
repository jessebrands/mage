//
// Created by Jesse on 12/28/2023.
//

#pragma once

#include <atomic>
#include <cassert>
#include <stdexcept>
#include <windows.h>

namespace mage {
    /*!
     * \brief Provides a convenience wrapper to retrieve an ANSI/UNICODE window class name string literal.
     * \tparam N Size of the string. (deduced)
     */
    template<size_t N>
    struct win32_window_class_name {
        TCHAR value[N]{};

        constexpr win32_window_class_name(const TCHAR (&name)[N]) {
            std::copy_n(name, N, value);
        }
    };


    /*!
     * This class provides a convenient abstraction of Win32's rather complicated Window Class semantics.
     *
     * To use this class, simply inherit using the CRTP pattern, supplying the inherting class as the template argument
     * T of this class:
     *
     * <pre>
     * class my_window : win32_window_class<my_window, TEXT("MyWindow")>
     * </pre>
     *
     * This base class will handle reference counting for the unique implementing Window class. The
     * only requirement is that the derived class has a static method called <code>window_proc</code>, with the exact
     * same function signature as the Win32 <code>WNDPROC</code> type.
     *
     * If you define this function as a private or protected method of the implementing class, then make sure to
     * declare win32_window_class as a friend class. The implementing class T will always be declared as a friend of
     * the base class, allowing access to its internals if you so desire.
     *
     * Everytime an instance of the implementing class is instantiated, this class will automatically register the
     * window class if needed, and keep a reference count of how many windows are using the class. Once all windows are
     * destroyed and the reference count reaches zero, the class is unregistered again.
     *
     * \brief Provides an abstraction over Win32 Window Class semantics.
     * \tparam T The implementation class for the window class.
     * \tparam N The name of the implementing Win32 window class.
     * \see https://learn.microsoft.com/en-us/windows/win32/winmsg/window-classes
     */
    template<class T, win32_window_class_name N>
    class win32_window_class {
        using type_t = win32_window_class;
        using window_t = T;

        friend window_t;

        static inline HINSTANCE instance_{nullptr};
        static inline ATOM atom_{0};
        static inline std::atomic_ulong counter_{0};

        static inline LPCTSTR window_class_name_ = N.value;

        /*!
         * \brief Registers a Window Class for the implementing class or increases the reference count for the class.
         * \see https://learn.microsoft.com/en-us/windows/win32/winmsg/using-window-classes
         */
        explicit win32_window_class(HINSTANCE instance = GetModuleHandle(nullptr),
                                    UINT style = CS_HREDRAW | CS_VREDRAW,
                                    HICON icon = LoadIcon(nullptr, IDI_APPLICATION),
                                    HCURSOR cursor = LoadCursor(nullptr, IDC_ARROW)) {
            // Check if this class is already registered...
            if (counter_ == 0 && atom_ == 0) {
                instance_ = instance;

                const WNDCLASSEX wc = {
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

                atom_ = RegisterClassEx(&wc);

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

        /*!
         * \brief Decrements the reference count and (if needed) unregisters the window class.
         * \see https://learn.microsoft.com/en-us/windows/win32/winmsg/window-classes
         */
        ~win32_window_class() {
            assert(counter_ != 0 && atom_ != 0);

            --counter_;
            if (counter_ == 0 && atom_ != 0) {
                UnregisterClass(window_class_name_, instance_);
                atom_ = 0;
                instance_ = nullptr;
            }
        }

        /*!
         * \brief Returns the Win32 window class' registration name.
         * \return ANSI string containing the class name.
         */
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
}
