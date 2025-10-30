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

template<size_t... Is>
constexpr bool
equal_tuples(Tuple auto const& l, Tuple auto const& r, std::index_sequence<Is...>) noexcept {
    using std::get;
    static_assert(noexcept((... && (get<Is>(l) == get<Is>(r)))));
    return (... && (get<Is>(l) == get<Is>(r)));
}

} // namespace detail

template<Tuple L, Tuple R>
constexpr auto equal_sequences(L const& l, R const& r) noexcept {
    if constexpr (std::tuple_size_v<L> != std::tuple_size_v<R>)
        return std::false_type{};
    else
        return detail::equal_tuples(l, r, tuple_indices_t<L>{});
}

} // namespace ac
