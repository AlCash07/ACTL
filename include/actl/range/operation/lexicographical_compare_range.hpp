// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/category/range.hpp>
#include <actl/meta/constant_literals.hpp>
#include <actl/operation/core/composite_operation.hpp>
#include <actl/operation/scalar/comparison/cmp3way.hpp>
#include <actl/operation/scalar/comparison/less.hpp>

namespace ac {

struct lexicographical_compare_range_f
{
    static constexpr index inner_count = 1;

    template <class Cmp3WayOp, class T, class U>
    static int evaluate(const Cmp3WayOp& op, const T& lhs, const U& rhs)
    {
        // Can't use std::lexicographical_compare because it doesn't compare
        // 3-way.
        auto lfirst = std::begin(lhs);
        auto llast = std::end(lhs);
        auto rfirst = std::begin(rhs);
        auto rlast = std::end(rhs);
        for (; (lfirst != llast) && (rfirst != rlast); ++lfirst, ++rfirst)
        {
            const int v = op(*lfirst, *rfirst);
            if (v != 0)
                return v;
        }
        return int{rfirst != rlast} - int{lfirst != llast};
    }
};
inline constexpr operation_composer<lexicographical_compare_range_f>
    lexicographical_compare_range;

template <class T, class U>
struct overload<cmp3way_f, range_tag, T, U>
{
    static constexpr auto formula =
        lexicographical_compare_range(resolve_nested<T, U>(cmp3way));
};

template <class T, class U>
struct overload<less_f, range_tag, T, U>
{
    static constexpr auto formula = cmp3way < 0_c;
};

} // namespace ac
