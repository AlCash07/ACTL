// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/geometry/algorithm/area/point_point.hpp>
#include <actl/geometry/algorithm/intersect/intersect.hpp>
#include <actl/geometry/detail/common_line.hpp>
#include <actl/geometry/detail/scalar_to_point_adaptor.hpp>

namespace ac {

/**
 * This policy implies that lines are in general position (don't coincide but can be parallel).
 */
template <class Policy>
struct general_position_policy {
    explicit general_position_policy(const Policy& x) : policy{x} {}

    const Policy& policy;
};

namespace detail {

template <class Policy, class T0, class K0, class T1, class K1, class T2>
bool cross_test(const Policy& policy, const line<T0, 2, K0>& lhs, const line<T1, 2, K1>& rhs,
                T2 tarea, T2 larea, T2 rarea) {
    if (less(policy, tarea, 0)) {
        tarea = -tarea;
        larea = -larea;
        rarea = -rarea;
    }
    return line_test(policy, lhs.kind(), larea, tarea) &&
           line_test(policy, rhs.kind(), rarea, tarea);
}

}  // namespace detail

template <class Policy, class T0, class K0, class T1, class K1, class OutIter>
OutIter intersect(line_scalar_policy<Policy> lsp, const line<T0, 2, K0>& lhs,
                  const line<T1, 2, K1>& rhs, OutIter dst) {
    auto& policy = lsp.policy;
    auto tarea = area(policy, rhs.vector, lhs.vector);
    if (equal(policy, tarea, 0))
        return dst;
    auto v = lhs.begin - rhs.begin;
    auto larea = area(policy, v, rhs.vector);
    auto rarea = area(policy, v, lhs.vector);
    if (!detail::cross_test(policy, lhs, rhs, tarea, larea, rarea))
        return dst;
    *dst++ = ratio(policy, larea, tarea);
    return dst;
}

template <class Policy, class T0, class K0, class T1, class K1, class OutIter>
OutIter intersect(general_position_policy<Policy> gpp, const line<T0, 2, K0>& lhs,
                  const line<T1, 2, K1>& rhs, OutIter dst) {
    return intersect(line_scalar_policy{gpp.policy}, lhs, rhs,
                     detail::scalar_to_point_adaptor{lhs, dst});
}

template <class Policy, class T0, class K0, class T1, class K1, class OutIter>
OutIter intersect(const Policy& policy, const line<T0, 2, K0>& lhs, const line<T1, 2, K1>& rhs,
                  OutIter dst) {
    auto tarea = area(policy, rhs.vector, lhs.vector);
    auto v = lhs.begin - rhs.begin;
    auto larea = area(policy, v, rhs.vector);
    if (equal(policy, tarea, 0)) {
        if (!equal(policy, larea, 0))
            return dst;
        return detail::common_line(policy, lhs, rhs, dst);
    }
    auto rarea = area(policy, v, lhs.vector);
    if (!detail::cross_test(policy, lhs, rhs, tarea, larea, rarea))
        return dst;
    auto p = lhs(policy, ratio(policy, larea, tarea));
    *dst++ = make_any_line(p, decltype(p){}, line_kind::closed_segment, true);
    return dst;
}

}  // namespace ac
