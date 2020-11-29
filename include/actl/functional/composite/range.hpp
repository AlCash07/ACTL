/***************************************************************************************************
 * Copyright 2020 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/functional/composite/composite_operation.hpp>
#include <actl/functional/scalar/comparison.hpp>
#include <actl/range/traits/is_range.hpp>
#include <algorithm>

namespace ac::math {

struct range_tag {
    struct has_nested;
};

template <class T>
// struct category_impl<T, std::enable_if_t<is_range_v<T> && !is_contiguous_range_v<T>>> {
struct category_impl<T, std::enable_if_t<is_range_v<T>>> {
    using type = range_tag;
};

struct EqualRange {
    template <class EqualOp, class T, class U>
    static bool evaluate(const EqualOp& op, const T& lhs, const U& rhs) {
        return std::equal(std::begin(lhs), std::end(lhs), std::begin(rhs), std::end(rhs), op);
    }
};
constexpr operation_composer<EqualRange> equal_range;

template <class T, class U>
struct overload<Equal, range_tag, T, U> {
    static constexpr auto resolve(Equal op) { return equal_range(op.resolve_nested<T, U>()); }
};

struct LexicographicalCompareRange {
    template <class Cmp3WayOp, class T, class U>
    static int evaluate(const Cmp3WayOp& op, const T& lhs, const U& rhs) {
        // Can't use std::lexicographical_compare because it doesn't compare 3-way.
        auto lfirst = std::begin(lhs);
        auto llast = std::end(lhs);
        auto rfirst = std::begin(rhs);
        auto rlast = std::end(rhs);
        for (; (lfirst != llast) && (rfirst != rlast); ++lfirst, ++rfirst) {
            const int v = op(*lfirst, *rfirst);
            if (v != 0) return v;
        }
        return cast<int>(rfirst != rlast) - cast<int>(lfirst != llast);
    }
};
constexpr operation_composer<LexicographicalCompareRange> lexicographical_compare_range;

template <class T, class U>
struct overload<Cmp3Way, range_tag, T, U> {
    static constexpr auto resolve(Cmp3Way op) {
        return lexicographical_compare_range(op.resolve_nested<T, U>());
    }
};

template <class T, class U>
struct overload<Less, range_tag, T, U> {
    static constexpr auto resolve(Less) { return cmp3way < 0; }
};

}  // namespace ac::math
