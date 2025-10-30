// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/geometry/algorithm/area/point_point.hpp>
#include <actl/geometry/algorithm/intersect/intersect.hpp>
#include <actl/geometry/detail/common_line.hpp>
#include <actl/geometry/detail/scalar_to_point_adaptor.hpp>

namespace ac {

/// This policy implies that lines are in general position (don't coincide but
/// can be parallel).
template<typename P>
struct general_position_policy {
    explicit general_position_policy(P const& x) : policy{x} {}

    P const& policy;
};

namespace detail {

template<typename TL, typename KL, typename TR, typename KR, typename TA>
bool cross_test(
    Policy auto const& policy,
    line<TL, 2, KL> const& l,
    line<TR, 2, KR> const& r,
    TA tarea,
    TA larea,
    TA rarea
) {
    if (less(policy, tarea, 0)) {
        tarea = -tarea;
        larea = -larea;
        rarea = -rarea;
    }
    return line_test(policy, l.kind(), larea, tarea) &&
           line_test(policy, r.kind(), rarea, tarea);
}

} // namespace detail

template<typename TL, typename KL, typename TR, typename KR, typename OutIter>
OutIter intersect(
    line_scalar_policy<P> lsp,
    line<TL, 2, KL> const& lhs,
    line<TR, 2, KR> const& rhs,
    OutIter dst
) {
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

template<typename TL, typename KL, typename TR, typename KR, typename OutIter>
OutIter intersect(
    general_position_policy<P> gpp,
    line<TL, 2, KL> const& l,
    line<TR, 2, KR> const& r,
    OutIter dst
) {
    return intersect(
        line_scalar_policy{gpp.policy},
        l,
        r,
        detail::scalar_to_point_adaptor{l, dst}
    );
}

template<typename TL, typename KL, typename TR, typename KR, typename OutIter>
OutIter intersect(
    Policy auto const& policy,
    line<TL, 2, KL> const& lhs,
    line<TR, 2, KR> const& rhs,
    OutIter dst
) {
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

} // namespace ac
