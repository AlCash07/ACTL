// Copyright 2022 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/container/equal_sequences/ranges.hpp>
#include <actl/container/equal_sequences/tuples.hpp>

namespace ac {

namespace detail {

template <class T, class U, size_t... Is>
constexpr bool equal_tuple_range(
    T const& lhs, U const& rhs, std::index_sequence<Is...>) noexcept
{
    using std::get;
    static_assert(noexcept((... && (get<Is>(lhs) == rhs[Is]))));
    return (... && (get<Is>(lhs) == rhs[Is]));
}

} // namespace detail

template <Tuple T, class U, enable_int_if<is_dynamic_range_v<U>> = 0>
constexpr bool equal_sequences(T const& lhs, U const& rhs) noexcept
{
    static_assert(RandomAccessRange<U>);
    constexpr size_t n = std::tuple_size_v<T>;
    static_assert(noexcept(rhs.size()));
    if (rhs.size() != n)
        return false;
    return detail::equal_tuple_range(lhs, rhs, std::make_index_sequence<n>{});
}

template <
    class T,
    class U,
    enable_int_if<is_dynamic_range_v<T> && Tuple<U>> = 0>
constexpr bool equal_sequences(T const& lhs, U const& rhs) noexcept
{
    return equal_sequences(rhs, lhs);
}

} // namespace ac
