// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/core/if_else.hpp>
#include <actl/numeric/comparison/compare_3way.hpp>
#include <actl/numeric/comparison/equality.hpp>
#include <actl/numeric/comparison/ordering.hpp>
#include <actl/sequence/tuple/at_tuple.hpp>

namespace ac {

template<size_t Index, size_t Length>
constexpr auto lexicographical_compare_tuple_suffix() noexcept {
    auto compare_element = [] {
        return compare_3way(
            at_tuple(l_, constant<Index>{}), //
            at_tuple(r_, constant<Index>{})
        );
    };
    if constexpr (Index + 1 == Length)
        return compare_element();
    else
        return if_else(l_ != 0, l_, r_)(
            compare_element(),
            lexicographical_compare_tuple_suffix<Index + 1, Length>()
        );
}

template<Tuple L, Tuple R>
constexpr auto specialization(Compare3Way, type_array<L, R>) noexcept {
    // TODO: consider supporting tuples of different sizes.
    static_assert(std::tuple_size_v<L> == std::tuple_size_v<R>);
    return lexicographical_compare_tuple_suffix<0, std::tuple_size_v<L>>();
}

template<Tuple L, Tuple R>
constexpr auto specialization(IsLess, type_array<L, R>) noexcept {
    return compare_3way < 0_c;
}

} // namespace ac
