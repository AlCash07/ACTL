// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/geometry/detail/line_test.hpp>
#include <actl/std/utility.hpp>
#include <functional>

namespace ac::detail {

template<typename T>
auto const& max_endpoint(
    Policy auto const& policy,
    std::pair<T, endpoint> const& lhs,
    std::pair<T, endpoint> const& rhs,
    bool start
) {
    if (lhs.second == endpoint::free) {
        if (rhs.second != endpoint::free) {
            return rhs;
        } else {
            return less(policy, lhs.first, rhs.first) == start ? lhs : rhs;
        }
    } else {
        if (rhs.second == endpoint::free) {
            return lhs;
        } else {
            int v = compare3way(policy, lhs.first, rhs.first);
            if (v == 0)
                return lhs.second >= rhs.second ? lhs : rhs;
            return v > 0 == start ? lhs : rhs;
        }
    }
}

template<
    index N,
    typename TL,
    typename KL,
    typename TR,
    typename KR,
    typename OutIter>
OutIter common_line(
    Policy auto const& policy,
    line<TL, N, KL> const& l,
    line<TR, N, KR> const& r,
    OutIter output
) {
    using point_t = point<geometry::scalar_t<TL, TR>, N>;
    auto get_point = [&policy](auto const& l, bool start) {
        return less(policy, point<int, N>{}, l.vector) == start
                   ? std::pair{point_t{l.begin}, begin(l.kind())}
                   : std::pair{point_t{l.end()}, end(l.kind())};
    };
    auto la =
        max_endpoint(policy, get_point(l, true), get_point(r, true), true);
    auto lb =
        max_endpoint(policy, get_point(l, false), get_point(r, false), false);
    if (endpoint_test(policy, la.second, la.first, lb.first)) {
        *output++ = make_any_line(la.first, la.second, lb.first, lb.second);
    }
    return output;
}

} // namespace ac::detail
