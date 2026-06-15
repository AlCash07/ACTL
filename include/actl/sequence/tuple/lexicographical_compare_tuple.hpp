// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/core/if_else.hpp>
#include <actl/numeric/comparison/compare3way.hpp>
#include <actl/numeric/comparison/equality.hpp>
#include <actl/numeric/comparison/ordering.hpp>
#include <actl/sequence/tuple/at_tuple.hpp>

namespace ac {

template<Tuple L, Tuple R, size_t Index>
struct lexicographical_compare_tuple_suffix {
    static constexpr auto compare_element() {
        return compare3way(
            at(l_, ac::constant<Index>{}), //
            at(r_, ac::constant<Index>{})
        );
    }

    static constexpr auto formula = [] {
        if constexpr (Index + 1 == std::tuple_size_v<L>)
            return compare_element();
        else
            return if_else(l_ != 0, l_, r_)(
                compare_element(),
                lexicographical_compare_tuple_suffix<L, R, Index + 1>::formula
            );
    }();
};

template<Tuple L, Tuple R>
struct specialization<Compare3Way, L, R>
    : lexicographical_compare_tuple_suffix<L, R, 0> {
    // TODO: consider supporting tuples of different sizes.
    static_assert(std::tuple_size_v<L> == std::tuple_size_v<R>);
};

template<Tuple L, Tuple R>
struct specialization<IsLess, L, R> {
    static constexpr auto formula = compare3way < 0_c;
};

} // namespace ac
