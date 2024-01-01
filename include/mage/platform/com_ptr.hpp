//
// Created by Jesse on 1/1/2024.
//

#pragma once

#include <cassert>
#include <type_traits>
#include <format>

#include <unknwn.h>

namespace mage {
    template<typename T> requires std::is_base_of_v<IUnknown, T>
    class com_ptr final {
        using type_t = T;

        type_t* ptr_;

    public:
        /*!
         * \brief Initializes this pointer as null.
         */
        com_ptr() noexcept : ptr_(nullptr) {
            // Intentionally left blank.
        }

        /*!
         * \brief Takes ownership of T, managing it. Does not call AddRef.
         * \param ptr COM pointer that is T or derived of T.
         */
        explicit com_ptr(type_t* ptr) noexcept : ptr_(ptr) {
            // intentionally left blank
#ifdef _DEBUG
            std::cout
                    << std::format("Taking ownership of {}", typeid(type_t).name())
                    << std::endl;
#endif
        }

        template<typename D> requires std::is_base_of_v<T, D>
        explicit com_ptr(D* ptr) noexcept : ptr_(ptr) {
#ifdef _DEBUG
            std::cout
                    << std::format("Taking ownership of {}, downcasting from {}", typeid(T).name(), typeid(D).name())
                    << std::endl;
#endif
        }

        /*!
         * \brief Creates a copy of a COM pointer. Calls AddRef.
         * \param other The other COM pointer.
         */
        com_ptr(const com_ptr& other) noexcept : ptr_(other.ptr_) {
            if (ptr_ != nullptr) {
                ptr_->AddRef();
            }
        }

        template<typename D> requires std::is_base_of_v<T, D>
        explicit com_ptr(const com_ptr<D>& other) noexcept : ptr_(other.ptr_) {
            if (ptr_ != nullptr) {
                ptr_->AddRef();
            }
        }

        /*!
         * \brief Move constructs a COM pointer from another COM point. Does not call AddRef.
         * \param other The COM pointer to be moved into this one.
         */
        com_ptr(com_ptr&& other) noexcept : ptr_(other.ptr_) {
            other.ptr_ = nullptr;
        }

        template<typename D> requires std::is_base_of_v<T, D>
        explicit com_ptr(com_ptr&& other) noexcept : ptr_(other.ptr_) {
            other.ptr_ = nullptr;
        }

        /*!
         * \brief Releases this COM pointer.
         */
        ~com_ptr() noexcept {
            if (ptr_ != nullptr) {
                ptr_->Release();
#ifdef _DEBUG
                std::cout
                        << std::format("Release {}", typeid(type_t).name())
                        << std::endl;
#endif
            }
        }

        /*!
         * \brief Copy assigns a COM pointer into this one. Calls Release and AddRef.
         * \param other The COM pointer to be assigned to this one.
         * \return This COM pointer.
         */
        com_ptr& operator=(const com_ptr& other) noexcept {
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
        com_ptr& operator=(com_ptr&& other) noexcept {
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
        constexpr auto operator->() const noexcept -> type_t* {
            assert(ptr_ != nullptr);
            return ptr_;
        }

        [[nodiscard]]
        constexpr auto get() const noexcept -> type_t* {
            return ptr_;
        }

        [[nodiscard]]
        constexpr auto get() noexcept -> type_t* {
            return ptr_;
        }

        [[nodiscard]]
        constexpr auto get_address_of() const noexcept -> const void** {
            return reinterpret_cast<const void **>(&ptr_);
        }

        [[nodiscard]]
        constexpr auto get_address_of() noexcept -> void** {
            return reinterpret_cast<void **>(&ptr_);
        }
    };
}
