/***************************************************************************************************
 * Integer ranges.
 * Allow to replace loops like `for (int i = a; i < b; ++i)` with `for (int i : irange(a, b))` etc.
 ***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/iterator/integer_iterator.hpp>
#include <actl/range/range.hpp>

namespace ac {

/**
 * Increasing range from @p first to @p last (not inclusive).
 */
template <class Int>
inline auto irange(Int first, Int last) {
    auto begin = integer_iterator<Int>(first);
    if (first > last) return make_range(begin, begin);
    return make_range(begin, integer_iterator<Int>(last));
}

/**
 * Increasing range from 0 to @p last (not inclusive).
 */
template <class Int>
inline auto irange(Int last) { return irange(Int{}, last); }

/**
 * Decreasing range from @p first to @p last (inclusive).
 */
template <class Int>
inline auto drange(Int first, Int last) {
    auto begin = integer_iterator<Int, false>(first);
    if (first < last) return make_range(begin, begin);
    return make_range(begin, integer_iterator<Int, false>(last - 1));
}

/**
 * Decreasing range from @p first to 0 (inclusive).
 */
template <class Int>
inline auto drange(Int first) { return drange(first, Int{}); }

/**
 * Range with given @p step from @p first to @p last (inclusive if @p step is negative).
 */
template <class Int, class StepType>
inline auto irange(Int first, Int last, StepType step) {
    ACTL_ASSERT(step != 0);
    auto begin = integer_iterator_with_step<Int>(first, step);
    if (step > 0 ? (last < first) : (last > first)) return make_range(begin, begin);
    Int step_count = (last - first - (step > 0)) / step + 1;
    return make_range(begin, integer_iterator_with_step<Int>(first + step * step_count, step));
}

}  // namespace ac
