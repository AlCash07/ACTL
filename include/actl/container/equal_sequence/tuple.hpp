// Copyright 2022 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/category/tuple.hpp>
#include <actl/meta/type_traits.hpp>
#include <tuple>

namespace ac {

namespace detail {

template <class T, class U, size_t... Is>
constexpr bool equal_tuple(
    T const& lhs, U const& rhs, std::index_sequence<Is...>) noexcept
{
    using std::get;
    static_assert(noexcept((... && (get<Is>(lhs) == get<Is>(rhs)))));
    return (... && (get<Is>(lhs) == get<Is>(rhs)));
}

} // namespace detail

template <class T, class U, enable_int_if<is_tuple_v<T> && is_tuple_v<U>> = 0>
constexpr auto equal_sequence(T const& lhs, U const& rhs) noexcept
{
    if constexpr (std::tuple_size_v<T> != std::tuple_size_v<U>)
        return std::false_type{};
    else
        return detail::equal_tuple(
            lhs, rhs, std::make_index_sequence<std::tuple_size_v<T>>{});
}

} // namespace ac
