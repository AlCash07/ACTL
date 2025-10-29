// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/meta/constant_literals.hpp>
#include <actl/operation/core/composite_operation.hpp>
#include <actl/operation/scalar/comparison/cmp3way.hpp>
#include <actl/operation/scalar/comparison/less.hpp>
#include <actl/range/traits/nesting_depth.hpp>

namespace ac {

struct lexicographical_compare_range_f {
    static constexpr size_t inner_count = 1;

    template<typename Cmp3WayOp, typename T, typename U>
    static int evaluate(Cmp3WayOp const& op, T const& lhs, U const& rhs) {
        // Can't use std::lexicographical_compare because it doesn't compare
        // 3-way.
        auto lfirst = ranges::begin(lhs);
        auto llast = ranges::end(lhs);
        auto rfirst = ranges::begin(rhs);
        auto rlast = ranges::end(rhs);
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

template<Range T, Range U>
struct overload<cmp3way_f, T, U> {
    static constexpr auto formula =
        lexicographical_compare_range(resolve_nested<T, U>(cmp3way));
};

template<Range T, Range U>
struct overload<less_f, T, U> {
    static constexpr auto formula = cmp3way < 0_c;
};

} // namespace ac
