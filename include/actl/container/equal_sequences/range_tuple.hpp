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

template<typename L, typename R, size_t... Is>
constexpr bool
equal_tuple_range(L const& l, R const& r, std::index_sequence<Is...>) noexcept {
    using std::get;
    static_assert(noexcept((... && (get<Is>(l) == r[Is]))));
    return (... && (get<Is>(l) == r[Is]));
}

} // namespace detail

template<Tuple L, typename R>
    requires(is_dynamic_range_v<R>)
constexpr bool equal_sequences(L const& l, R const& r) noexcept {
    static_assert(RandomAccessRange<R>);
    constexpr size_t n = std::tuple_size_v<L>;
    static_assert(noexcept(r.size()));
    if (r.size() != n)
        return false;
    return detail::equal_tuple_range(l, r, std::make_index_sequence<n>{});
}

template<typename L, typename R>
    requires(is_dynamic_range_v<L> && Tuple<R>)
constexpr bool equal_sequences(L const& l, R const& r) noexcept {
    return equal_sequences(r, l);
}

} // namespace ac
