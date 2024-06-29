// Copyright 2021 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <cstddef>
#include <type_traits>

namespace ac {

/// ac::constant is an analog of `std::integral_constant` that provides a member
/// constant value equal to @p X. The difference is:
/// - Value type doesn't have to be specified explicitly. Instead of
/// `std::integral_constant<color, color::yellow>` one can write
/// `ac::constant<color::yellow>` to avoid repretition.
/// - As a result, compilation errors are shorter and easier to read.
/// - TODO:
/// - All constexpr operations applied to ac::constant also produce a constant.
/// For example, `-ac::constant<1>{}` results in `ac::constant<-1>{}`.
template <auto X>
struct constant : std::integral_constant<decltype(X), X> {
    friend constexpr constant<true> operator==(constant, constant) noexcept {
        return {};
    }
};

template <bool B>
using bool_constant = constant<B>;

template <bool B>
inline constexpr auto bool_c = bool_constant<B>{};

template <int N>
using int_constant = constant<N>;

template <int N>
inline constexpr auto int_c = int_constant<N>{};

template <size_t N>
using size_constant = constant<N>;

template <size_t N>
inline constexpr auto size_c = size_constant<N>{};

/// Provides a member constant bool value indicating
/// whether @p T is a specialization of ac::constant.
template <class T>
struct is_constant : std::false_type {};

template <auto X>
struct is_constant<constant<X>> : std::true_type {};

template <class T>
inline constexpr bool is_constant_v = is_constant<T>::value;

template <class T>
struct unwrap_constant {
    using type = T;
};

template <auto X>
struct unwrap_constant<constant<X>> {
    using type = decltype(X);
};

/// Provides the type of the underlying constant value.
/// Types other than ac::constant are unchanged.
template <class T>
using unwrap_constant_t = typename unwrap_constant<T>::type;

} // namespace ac
