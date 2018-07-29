/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/geometry/algorithm/crosses/line_line_2d.hpp>
#include <actl/geometry/detail/scalar_to_point_adaptor.hpp>

namespace ac {

template <class AreaPolicy = comparable_area_points<>>
struct intersect_line_line : AreaPolicy {};

template <class P = use_default>
using comparable_intersect_line_line = intersect_line_line<comparable_area_points<P>>;

template <class P = use_default, class S = use_default>
using standard_intersect_line_line = intersect_line_line<standard_area_points<P, S>>;

namespace detail {

template <class AP, class T0, class K0, class T1, class K1, class It>
inline auto intersect(const intersect_line_line<AP>& policy, const line<T0, 2, K0>& lhs,
                      const line<T1, 2, K1>& rhs, It dst, scalar_tag) {
    auto pair = intersect_lines_general<geometry::scalar_t<output_type_t<It>>>(policy, lhs, rhs);
    if (pair.first) *dst++ = pair.second;
    return dst;
}

template <class AP, class T0, class K0, class T1, class K1, class It>
inline auto intersect(const intersect_line_line<AP>& policy, const line<T0, 2, K0>& lhs,
                      const line<T1, 2, K1>& rhs, It dst, point_tag) {
    return intersect(policy, lhs, rhs, detail::adapt_iterator(lhs, dst), scalar_tag());
}

template <class AP, class T0, class K0, class T1, class K1, class It>
inline auto intersect(const intersect_line_line<AP>& policy, const line<T0, 2, K0>& lhs,
                      const line<T1, 2, K1>& rhs, It dst, line_tag) {
    auto pair = intersect_lines<geometry::scalar_t<output_type_t<It>>>(policy, lhs, rhs);
    if (pair.first) *dst++ = pair.second;
    return dst;
}

}  // namespace detail

template <class AP, class T0, class K0, class T1, class K1, class OutputIterator>
inline auto intersect(const intersect_line_line<AP>& policy, const line<T0, 2, K0>& lhs,
                      const line<T1, 2, K1>& rhs, OutputIterator dst) {
    return detail::intersect(policy, lhs, rhs, dst,
                             geometry::tag_t<output_type_t<OutputIterator>>());
}

template <class T0, class K0, class T1, class K1, class OutputIterator>
inline auto intersect(use_default, const line<T0, 2, K0>& lhs, const line<T1, 2, K1>& rhs,
                      OutputIterator dst) {
    return intersect(comparable_intersect_line_line<>(), lhs, rhs, dst);
}

}  // namespace ac
