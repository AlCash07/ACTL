// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/functional/noexcept/deduce_noexcept_requires_and_return.hpp>
#include <memory> // for std::addressof
#include <type_traits>
#include <utility> // for std::forward

namespace ac {

namespace detail {
template<class T>
struct is_out : std::false_type {};
} // namespace detail

/// Checks whether T is a (possibly cvref-qualified) ac::out wrapper.
/// @note If the program adds specializations for ac::is_inout_v,
/// the behavior is undefined.
template<class T>
inline constexpr bool is_out_v = detail::is_out<std::remove_cvref_t<T>>::value;

/// Thin wrapper over a reference-like or a pointer-like type
/// to specify an output-only function parameter.
///
/// It helps to simplify reasoning about the code behavior
/// by making potential variable modifications clearly visible
/// at both the function definition and all the call sites.
///
/// For example, in operations similar to copy it's not immediately clear
/// which of the arguments we're copying to:
/// ```
/// copy_range(x, y);
/// ```
/// Using ac::out wrapper, we can make it obvious
/// without any extra documentation:
/// ```
/// template<class Range>
/// void copy_range(out<Range&> dst, Range const& src) { ... }
///
/// copy_range(out{x}, y);
/// ```
template<class T>
class out {
public:
    /// Constructor from an arbitrary type convertible to the wrapped type.
    template<class Arg>
        requires(std::is_constructible_v<T, Arg &&> && !is_out_v<Arg>)
    explicit constexpr out(Arg&& arg
    ) noexcept(std::is_nothrow_constructible_v<T, Arg&&>)
        : x{std::forward<Arg>(arg)} {}

    /// Function arguments in C++ can undergo an implicit conversion
    /// to match the parameter types.
    /// For example, `std::vector<int>&` to `std::span<int>`.
    ///
    /// This implicit constructor allows to preserve this behavior
    /// without the extra manual conversion when ac::out wrapper is used
    /// if the wrapped types are implicitly convertible.
    /// For example, `ac::out<std::vector<int>&>` argument can be passed
    /// where `ac::out<std::span<int>>` parameter is expected.
    template<class U>
        requires(std::is_convertible_v<U, T>)
    constexpr out(out<U>&& src) noexcept(std::is_nothrow_constructible_v<T, U>)
        : x{*src} {}

    /// Accesses the wrapped value.
    /// @note operator* is used for consistency with `std::optional`.
    /// Simple operator like this is even more appropriate here than in
    /// `std::optional`, because `std::optional` might not contain a value,
    /// but here it's always available.
    constexpr T operator*() const noexcept {
        return x;
    }

    /// Implicit conversion to the wrapped type.
    constexpr operator T() const noexcept {
        return **this;
    }

    /// Provides direct access to the members of the wrapped type
    /// as `out->member`.
    /// @note It would be better to support `out.member` syntax,
    /// but C++ doesn't allow to overload `operator.` yet.
    constexpr std::remove_reference_t<T>* operator->() noexcept {
        return std::addressof(x);
    }

    /// operator= enables assignment extension by overloading
    /// the `assign` free function accessible via ADL.
    ///
    /// For example, if a third-party `Container` doesn't support
    /// assignment from a range like
    /// ```
    /// dstContainer = srcRange;
    /// ```
    /// we can still achieve a similar syntax
    /// ```
    /// ac::out{dstContainer} = srcRange;
    /// ```
    /// by implementing the `assign` function
    /// in the same namespace as Container:
    /// ```
    /// template<class InRange>
    /// void assign(ac::out<Container&> dst, InRange const& src) { ... }
    /// ```
    template<class Src>
    constexpr out& operator=(Src&& src) AC_DEDUCE_NOEXCEPT_REQUIRES_AND_RETURN(
        assign(*this, std::forward<Src>(src)), *this
    )

    // TODO: struct enable_operators;

private:
    T x;
};

template<class T>
out(T&&) -> out<T>;

namespace detail {
template<class T>
struct is_out<out<T>> : std::true_type {};
} // namespace detail

template<class Dst, class Src>
constexpr Dst assign(out<Dst>& dst, Src&& src)
    AC_DEDUCE_NOEXCEPT_REQUIRES_AND_RETURN(*dst = std::forward<Src>(src))

} // namespace ac
