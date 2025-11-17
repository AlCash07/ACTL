// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/meta/constant_literals.hpp>
#include <actl/operation/operation/composite_operation.hpp>
#include <actl/operation/scalar/comparison/cmp3way.hpp>
#include <actl/operation/scalar/comparison/less.hpp>
#include <actl/range/traits/nesting_depth.hpp>

namespace ac {

struct lexicographical_compare_range_f {
    static constexpr size_t inner_count = 1;

    template<typename Cmp3WayOp, typename L, typename R>
    static int evaluate(Cmp3WayOp const& op, L const& l, R const& r) {
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
inline constexpr operation_composer<lexicographical_compare_range_f>
    lexicographical_compare_range;

template<Range L, Range R>
struct overload<cmp3way_f, L, R> {
    static constexpr auto formula =
        lexicographical_compare_range(resolve_nested<L, R>(cmp3way));
};

template<Range L, Range R>
struct overload<less_f, L, R> {
    static constexpr auto formula = cmp3way < 0_c;
};

} // namespace ac
