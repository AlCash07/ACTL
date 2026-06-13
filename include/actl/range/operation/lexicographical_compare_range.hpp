// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/numeric/comparison/compare3way.hpp>
#include <actl/numeric/comparison/ordering.hpp>
#include <actl/numeric/constant_literals.hpp>
#include <actl/operation/type_operation.hpp>
#include <actl/range/traits/associated_types.hpp>

namespace ac {

struct LexicographicalCompareRange
    : operation_base<LexicographicalCompareRange> {
    static constexpr bool is_argument_operation(size_t index) {
        return index == 2;
    }

    static constexpr int evaluate(
        Range auto& l, Range auto& r, auto const& compare3way_element
    ) {
        // Can't use std::lexicographical_compare because it doesn't compare
        // 3-way.
        auto lfirst = ranges::begin(l);
        auto llast = ranges::end(l);
        auto rfirst = ranges::begin(r);
        auto rlast = ranges::end(r);
        for (; (lfirst != llast) && (rfirst != rlast); ++lfirst, ++rfirst) {
            int const v = compare3way_element(*lfirst, *rfirst);
            if (v != 0)
                return v;
        }
        return int{rfirst != rlast} - int{lfirst != llast};
    }
};

inline constexpr auto lexicographical_compare_range =
    LexicographicalCompareRange{}(
        l_,
        r_,
        compare3way(
            type_operation<range_reference, Arg<0, 2>>,
            type_operation<range_reference, Arg<1, 2>>
        )
    );

template<Range L, Range R>
struct specialization<IsLess, L, R> {
    static constexpr auto formula = compare3way < 0_c;
};

} // namespace ac
