// Copyright 2022 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <type_traits>

namespace ac {

namespace detail {

template <class Void, class T, class... Args>
struct can_list_initialize_impl : std::false_type
{};

template <class T, class... Args>
struct can_list_initialize_impl<
    std::void_t<decltype(T{std::declval<Args>()...})>,
    T,
    Args...> : std::true_type
{};

} // namespace detail

template <class T, class... Args>
inline constexpr bool can_list_initialize_v =
    detail::can_list_initialize_impl<void, T, Args...>::value;

} // namespace ac