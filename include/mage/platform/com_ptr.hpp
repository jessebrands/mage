//
// Created by Jesse on 1/1/2024.
//

#pragma once

#include <type_traits>
#include <unknwn.h>

namespace mage {
    /*!
     * <code>mage::com_ptr</code> is a smart pointer that owns and manages a COM object through a pointer. It assists
     * in reference counting and releasing the object when it goes out of scope.
     *
     * A <code>com_ptr</code> may alternatively own no object, in which case it is called empty.
     *
     * \brief Smart COM pointer.
     * \tparam T COM object interface that derives from IUnknown.
     */
    template<typename T> requires std::is_base_of_v<IUnknown, T>
    class com_ptr final {
        // Required to access ptr_ of other specializations.
        template<typename U> requires std::is_base_of_v<IUnknown, U>
        friend class com_ptr;

        T* ptr_;

    public:
        using type_t = com_ptr;
        using element_t = T;
        using pointer_t = T *;

        /*!
         * \brief Implicitly initializes this pointer as empty.
         */
        constexpr com_ptr() noexcept : ptr_(nullptr) {
            // Intentionally left blank.
        }

        /*!
         * \brief Explicitly initializes this pointer as empty.
         */
        explicit constexpr com_ptr(decltype(nullptr)) noexcept : ptr_(nullptr) {
            // Intentionally left blank.
        }

        /*!
         * \brief Takes ownership of U, managing it. Does not call AddRef.
         * \param ptr COM pointer that is T or U derived of T.
         */
        template<typename U> requires std::is_base_of_v<T, U>
        explicit constexpr com_ptr(U* ptr) noexcept : ptr_(ptr) {
            // Intentionally left blank.
        }

        /*!
         * \brief Creates a copy of a COM pointer. Calls AddRef.
         * \param other The other COM pointer.
         */
        com_ptr(const type_t& other) noexcept : ptr_(other.ptr_) {
            if (ptr_ != nullptr) {
                ptr_->AddRef();
            }
        }

        template<typename U> requires std::is_base_of_v<T, U>
        explicit com_ptr(const com_ptr<U>& other) noexcept : ptr_(other.ptr_) {
            if (ptr_ != nullptr) {
                ptr_->AddRef();
            }
        }

        /*!
         * \brief Move constructs a COM pointer from another COM point. Does not call AddRef.
         * \param other The COM pointer to be moved into this one.
         */
        constexpr com_ptr(type_t&& other) noexcept : ptr_(other.ptr_) {
            other.ptr_ = nullptr;
        }

        template<typename U> requires std::is_base_of_v<T, U>
        explicit constexpr com_ptr(com_ptr<U>&& other) noexcept : ptr_(other.ptr_) {
            other.ptr_ = nullptr;
        }

        /*!
         * \brief Releases this COM pointer.
         */
        ~com_ptr() noexcept {
            if (ptr_ != nullptr) {
                ptr_->Release();
            }
        }

        /*!
         * \brief Checks whether this owns an object.
         */
        explicit operator bool() const noexcept {
            return ptr_ != nullptr;
        }

        /*!
         * \brief Copy assigns a COM pointer into this one. Calls Release and AddRef.
         * \param other The COM pointer to be assigned to this one.
         * \return This COM pointer.
         */
        type_t& operator=(const type_t& other) noexcept {
            if (this != *other) {
                if (ptr_ != nullptr) {
                    ptr_->Release();
                }
                ptr_ = other.ptr_;
                if (ptr_ != nullptr) {
                    ptr_->AddRef();
                }
            }
            return *this;
        }

        /*!
         * \brief Move assigns a COM pointer into this one. Calls Release.
         * \param other The COM pointer to be move assigned.
         * \return This COM pointer.
         */
        type_t& operator=(type_t&& other) noexcept {
            if (this != *other) {
                if (ptr_ != nullptr) {
                    ptr_->Release();
                }
                ptr_ = other.ptr_;
                other.ptr_ = nullptr;
            }
            return *this;
        }

        /*!
         * \brief Provides direct access to the methods of this COM pointer.
         * \return The underlying pointer.
         */
        constexpr auto operator->() const noexcept -> pointer_t {
            return ptr_;
        }

        template<typename U> requires std::is_base_of_v<IUnknown, U>
        auto query_interface(com_ptr<U>& out) const noexcept -> HRESULT {
            if (out.ptr_ != nullptr) {
                out.ptr_->Release();
            }
            return ptr_->QueryInterface(
                __uuidof(U),
                reinterpret_cast<void **>(out.get_address_of())
            );
        }

        [[nodiscard]]
        constexpr auto get() const noexcept -> pointer_t {
            return ptr_;
        }

        [[nodiscard]]
        constexpr auto get_address_of() const noexcept -> pointer_t const* {
            return &ptr_;
        }

        [[nodiscard]]
        constexpr auto get_address_of() noexcept -> pointer_t* {
            return &ptr_;
        }
    };

    template<typename T> requires std::is_base_of_v<IUnknown, T>
    bool operator==(const com_ptr<T>& a, decltype(nullptr)) {
        return !a;
    }

    template<typename T> requires std::is_base_of_v<IUnknown, T>
    bool operator==(decltype(nullptr), const com_ptr<T>& a) {
        return !a;
    }
}
