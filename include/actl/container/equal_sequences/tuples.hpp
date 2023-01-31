// Copyright 2022 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/meta/tuple.hpp>
#include <tuple>

namespace ac {

namespace detail {

template <size_t... Is>
constexpr bool equal_tuples(
    Tuple auto const& lhs,
    Tuple auto const& rhs,
    std::index_sequence<Is...>) noexcept
{
    using std::get;
    static_assert(noexcept((... && (get<Is>(lhs) == get<Is>(rhs)))));
    return (... && (get<Is>(lhs) == get<Is>(rhs)));
}

} // namespace detail

template <Tuple T, Tuple U>
constexpr auto equal_sequences(T const& lhs, U const& rhs) noexcept
{
    if constexpr (std::tuple_size_v<T> != std::tuple_size_v<U>)
        return std::false_type{};
    else
        return detail::equal_tuples(lhs, rhs, tuple_indices_t<T>{});
}

} // namespace ac
