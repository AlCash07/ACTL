// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/geometry/detail/line_test.hpp>
#include <actl/std/utility.hpp>
#include <functional>

namespace ac::detail {

template <class Policy, class T>
inline const auto& max_endpoint(const Policy& policy, const std::pair<T, endpoint>& lhs,
                                const std::pair<T, endpoint>& rhs, bool start) {
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
            int v = cmp3way(policy, lhs.first, rhs.first);
            if (v == 0) return lhs.second >= rhs.second ? lhs : rhs;
            return v > 0 == start ? lhs : rhs;
        }
    }
}

template <class Policy, index N, class T0, class K0, class T1, class K1, class OutIter>
inline OutIter common_line(const Policy& policy, const line<T0, N, K0>& lhs,
                           const line<T1, N, K1>& rhs, OutIter dst) {
    using point_t = point<geometry::scalar_t<T0, T1>, N>;
    auto get_point = [&policy](const auto& l, bool start) {
        return less(policy, point<int, N>{}, l.vector) == start
                   ? std::pair{point_t{l.begin}, begin(l.kind())}
                   : std::pair{point_t{l.end()}, end(l.kind())};
    };
    auto la = max_endpoint(policy, get_point(lhs, true), get_point(rhs, true), true);
    auto lb = max_endpoint(policy, get_point(lhs, false), get_point(rhs, false), false);
    if (endpoint_test(policy, la.second, la.first, lb.first)) {
        *dst++ = make_any_line(la.first, la.second, lb.first, lb.second);
    }
    return dst;
}

}  // namespace ac::detail
