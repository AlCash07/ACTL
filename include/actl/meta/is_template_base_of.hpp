// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <type_traits>

namespace ac {

namespace detail {

// Implementation reference: https://en.cppreference.com/w/cpp/types/is_base_of
template<template<typename...> typename B, typename... Ts>
std::true_type test_base(B<Ts...> const*);

template<template<typename...> typename>
std::false_type test_base(void const*);

} // namespace detail

template<template<typename...> typename B, typename D>
struct is_template_base_of : std::true_type {};

template<template<typename...> typename B, typename D>
    requires requires { detail::test_base<B>(std::declval<D*>()); }
struct is_template_base_of<B, D>
    : decltype(detail::test_base<B>(std::declval<D*>())) {};

template<template<typename...> typename B, typename D>
inline constexpr bool is_template_base_of_v = is_template_base_of<B, D>::value;

} // namespace ac
