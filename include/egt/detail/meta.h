/*
 * Copyright (C) 2018 Microchip Technology Inc.  All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef EGT_DETAIL_META_H
#define EGT_DETAIL_META_H

/**
 * @file
 * @brief Meta tools for compilation, debug, and language support.
 */

#include <cmath>
#include <cstdlib>
#include <egt/detail/math.h>
#include <functional>
#include <memory>
#include <string>
#include <type_traits>
#include <utility>

#ifdef egt_likely
#  define egt_likely(x)       __builtin_expect((x), 1)
#endif

#ifndef egt_unlikely
#  define egt_unlikely(x)     __builtin_expect((x), 0)
#endif

#if defined _WIN32 || defined __CYGWIN__
#  define EGT_HELPER_DLL_IMPORT __declspec(dllimport)
#  define EGT_HELPER_DLL_EXPORT __declspec(dllexport)
#  define EGT_HELPER_DLL_LOCAL
#else
#  if __GNUC__ >= 4
#    define EGT_HELPER_DLL_IMPORT __attribute__ ((visibility ("default")))
#    define EGT_HELPER_DLL_EXPORT __attribute__ ((visibility ("default")))
#    define EGT_HELPER_DLL_LOCAL  __attribute__ ((visibility ("hidden")))
#  else
#    define EGT_HELPER_DLL_IMPORT
#    define EGT_HELPER_DLL_EXPORT
#    define EGT_HELPER_DLL_LOCAL
#  endif
#endif

// Now we use the generic helper definitions above to define EGT_API and EGT_LOCAL.
// EGT_API is used for the public API symbols. It either DLL imports or DLL exports (or does nothing for static build)
// EGT_LOCAL is used for non-api symbols.

#ifndef EGT_STATIC
#  define EGT_DLL
#endif

#ifdef EGT_DLL // defined if EGT is compiled as a DLL
#  ifdef EGT_DLL_EXPORTS // defined if we are building the EGT DLL (instead of using it)
#    define EGT_API EGT_HELPER_DLL_EXPORT
#  else
#    define EGT_API EGT_HELPER_DLL_IMPORT
#  endif // EGT_DLL_EXPORTS
#  define EGT_LOCAL EGT_HELPER_DLL_LOCAL
#else // EGT_DLL is not defined: this means EGT is a static lib.
#  define EGT_API
#  define EGT_LOCAL
#endif // EGT_DLL

#ifndef SWIG
#  define EGT_DEPRECATED [[deprecated]]
#  ifdef __has_cpp_attribute
#    define EGT_HAVE_ATTRIBUTE(x) __has_cpp_attribute(x)
#  else
#    define EGT_HAVE_ATTRIBUTE(x) 0
#  endif
#  if EGT_HAVE_ATTRIBUTE(nodiscard) && !defined(__clang__)
#    define EGT_NODISCARD [[nodiscard]]
#  elif EGT_HAVE_ATTRIBUTE(warn_unused_result)
#    define EGT_NODISCARD __attribute__((warn_unused_result))
#  else
#    define EGT_NODISCARD
#  endif
#else
#  define EGT_DEPRECATED
#  define EGT_NODISCARD
#endif

namespace egt
{
inline namespace v1
{
namespace detail
{

/**
 * Utility function to safely ignore a parameter to a function.
 */
template <typename T>
void ignoreparam(T&&) // NOLINT(readability-named-parameter)
{}

#ifndef SWIG
/**
 * Range class to work with C++11 range based for loops in a reverse order.
 * @see reverse_iterate
 */
template <typename T>
class ReverseRange
{
    T& m_x;
public:
    /**
     * @param x Container reference.
     */
    explicit ReverseRange(T& x) : m_x(x) {}

    /// begin iterator
    EGT_NODISCARD auto begin() const -> decltype(this->m_x.rbegin())
    {
        return m_x.rbegin();
    }

    /// end iterator
    EGT_NODISCARD auto end() const -> decltype(this->m_x.rend())
    {
        return m_x.rend();
    }
};

/**
 * Reverse iterator to work with C++11 range based for loops in a reverse order.
 *
 * @code{.cpp}
 * for (auto& child : detail::reverse_iterate(children))
 * {
 *     ...
 * }
 * @endcode
 */
template <typename T>
ReverseRange<T> reverse_iterate(T& x)
{
    return ReverseRange<T>(x);
}
#endif

/**
 * Utility base class to make a derived class non-copy-able.
 */
template<class T>
class EGT_API NonCopyable
{
public:
    NonCopyable(const NonCopyable&) = delete;
    NonCopyable& operator=(const NonCopyable&) = delete;
    NonCopyable(NonCopyable&&) noexcept = default;
    NonCopyable& operator=(NonCopyable&&) noexcept = default;
protected:
    NonCopyable() = default;
    ~NonCopyable() = default;
};

/**
 * Rule of 5 static assert all in one.
 *
 * Example usage is intended for static_assert:
 * @code{.cpp}
 * static_assert(detail::rule_of_5<Color>(), "must fulfill rule of 5");
 * @endcode
 */
template <class T>
constexpr bool rule_of_5()
{
    static_assert(std::is_destructible<T>::value, "must be destructible");
    static_assert(std::is_copy_constructible<T>::value, "must be copy constructible");
    static_assert(std::is_move_constructible<T>::value, "must be move constructible");
    static_assert(std::is_copy_assignable<T>::value, "must be copy assignable");
    static_assert(std::is_move_assignable<T>::value, "must be move assignable");

    return std::is_destructible<T>::value && std::is_copy_constructible<T>::value
           && std::is_move_constructible<T>::value && std::is_copy_assignable<T>::value
           && std::is_move_assignable<T>::value;
}

/**
 * Utility to test and change a value if it is different.
 *
 * This is a very common pattern used by Widget functions to determine whether
 * damage() should be called when a property is changed.
 *
 * @param old Reference to the old value.
 * @param to Potential new value.
 * @return true if the value was changed.
 */
template<class T>
constexpr bool change_if_diff(T& old, const T& to)
{
    if (old != to)
    {
        old = to;
        return true;
    }

    return false;
}

/**
 * @param old Reference to the old value.
 * @param to Potential new value.
 */
template<>
constexpr bool change_if_diff(float& old, const float& to)
{
    if (!detail::float_equal(old, to))
    {
        old = to;
        return true;
    }

    return false;
}

/**
 * @param old Reference to the old value.
 * @param to Potential new value.
 */
template<>
constexpr bool change_if_diff(double& old, const double& to)
{
    if (!detail::float_equal(old, to))
    {
        old = to;
        return true;
    }

    return false;
}

/**
 * Utility to run a callback when this object goes out of scope.
 *
 * This can be used to run a callable when an instance of a ScopeExit goes out
 * of scope or is deleted.
 */
template<class T>
struct EGT_API ScopeExit
{
    /**
     * @param f Callable to call on destruction.
     */
    // NOLINTNEXTLINE(hicpp-explicit-conversions)
    explicit ScopeExit(T&& f) noexcept
        : m_f(std::move(f)),
          m_active(true)
    {}

    /**
     * @param f Callable to call on destruction.
     */
    // NOLINTNEXTLINE(hicpp-explicit-conversions)
    explicit ScopeExit(const T& f) noexcept
        : m_f(f),
          m_active(true)
    {}

    /// Move constructor
    ScopeExit(ScopeExit&& other) noexcept
        : m_f(std::move(other.m_f)),
          m_active(other.m_active)
    {
        other.m_active = false;
    }

    ScopeExit() = delete;
    ScopeExit(const ScopeExit&) = delete;
    ScopeExit& operator=(const ScopeExit&) = delete;
    ScopeExit& operator=(ScopeExit&&) = delete;

    ~ScopeExit()
    {
        if (m_active)
            m_f();
    }

protected:
    /// Callback function.
    T m_f;
    /// Used to manage move
    bool m_active;
};

/**
 * Helper to construct a ScopeExit with proper type deduction of the template
 * parameter, which may be a lambda.
 */
template<class T>
ScopeExit<T> on_scope_exit(T&& f)
{
    return ScopeExit<T>(std::forward<T>(f));
}

/**
 * Utility to create a bit mask for the specified bit.
 *
 * @param[in] n The bit number.
 */
template<class T>
constexpr T bit(T n)
{
    return 1u << n;
}

}
}
}

#endif
