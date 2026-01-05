// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/meta/constant_literals.hpp>
#include <actl/numeric/comparison/compare3way.hpp>
#include <actl/numeric/comparison/ordering.hpp>
#include <actl/operation/operation/composite_operation.hpp>
#include <actl/range/traits/associated_types.hpp>

namespace ac {

struct LexicographicalCompareRange {
    static constexpr size_t inner_count = 1;

    template<typename Compare3WayOp, typename L, typename R>
    static int evaluate(Compare3WayOp const& op, L const& l, R const& r) {
        // Can't use std::lexicographical_compare because it doesn't compare
        // 3-way.
        auto lfirst = ranges::begin(l);
        auto llast = ranges::end(l);
        auto rfirst = ranges::begin(r);
        auto rlast = ranges::end(r);
        for (; (lfirst != llast) && (rfirst != rlast); ++lfirst, ++rfirst) {
            int const v = op(*lfirst, *rfirst);
            if (v != 0)
                return v;
        }
        return int{rfirst != rlast} - int{lfirst != llast};
    }
};
inline constexpr operation_composer<LexicographicalCompareRange>
    lexicographical_compare_range;

template<Range L, Range R>
struct overload<Compare3Way, L, R> {
    static constexpr auto formula = lexicographical_compare_range(
        resolve_overload<range_value_t<L>, range_value_t<R>>(
            default_context{}, compare3way
        )
    );
};

template<Range L, Range R>
struct overload<IsLess, L, R> {
    static constexpr auto formula = compare3way < 0_c;
};

} // namespace ac
