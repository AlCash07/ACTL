// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/category/range.hpp>
#include <actl/functional/composite/composite_operation.hpp>
#include <actl/functional/scalar/comparison.hpp>

namespace ac {

struct lexicographical_compare_range_t {
    template <class Cmp3WayOp, class T, class U>
    static int evaluate(const Cmp3WayOp& op, const T& lhs, const U& rhs) {
        // Can't use std::lexicographical_compare because it doesn't compare 3-way.
        auto lfirst = std::begin(lhs);
        auto llast = std::end(lhs);
        auto rfirst = std::begin(rhs);
        auto rlast = std::end(rhs);
        for (; (lfirst != llast) && (rfirst != rlast); ++lfirst, ++rfirst) {
            const int v = op(*lfirst, *rfirst);
            if (v != 0)
                return v;
        }
        return cast<int>(rfirst != rlast) - cast<int>(lfirst != llast);
    }
};
constexpr operation_composer<lexicographical_compare_range_t> lexicographical_compare_range;

template <class T, class U>
struct overload<cmp3way_t, range_tag, T, U> {
    static constexpr auto resolve(cmp3way_t op) {
        return lexicographical_compare_range(op.resolve_nested<T, U>());
    }
};

template <class T, class U>
struct overload<less_t, range_tag, T, U> {
    static constexpr auto resolve(less_t) {
        return cmp3way < zero_;
    }
};

}  // namespace ac
