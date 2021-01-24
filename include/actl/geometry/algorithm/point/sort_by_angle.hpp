// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/geometry/algorithm/orientation/point_point.hpp>
#include <actl/geometry/multi_point.hpp>
#include <algorithm>

namespace ac {

/// Sorts 2d points by polar angle from 0 to 2 * pi around the origin.
/// Points at the origin go first.
template <
    class Policy,
    class U,
    class T,
    enable_int_if<is_multi_point_v<U> && geometry_traits<U>::dimension == 2> =
        0>
void sort_by_angle(const Policy& policy, U& points, const point<T>& origin) {
    using ref = reference_t<U>;
    auto to_point = get_to_point(points);
    auto first = points.begin(), last = points.end();
    first = std::partition(first, last, [to_point, &policy, &origin](ref x) {
        return equal(policy, origin, to_point(x));
    });
    auto pivot =
        std::partition(first, last, [to_point, &policy, &origin](ref x) {
            return y_compare(policy, origin, to_point(x));
        });
    auto comp = [to_point, &policy](ref lhs, ref rhs) {
        return left_turn(policy, to_point(lhs), to_point(rhs));
    };
    std::sort(first, pivot, comp);
    std::sort(pivot, last, comp);
}

/// Sort by angle around (0, 0).
template <
    class Policy,
    class U,
    enable_int_if<is_multi_point_v<U> && geometry_traits<U>::dimension == 2> =
        0>
void sort_by_angle(const Policy& policy, U& points) {
    using ref = reference_t<U>;
    auto to_point = get_to_point(points);
    auto first = points.begin(), last = points.end();
    first = std::partition(first, last, [to_point, &policy](ref x) {
        return degenerate(policy, to_point(x));
    });
    auto pivot = std::partition(first, last, [to_point, &policy](ref x) {
        return y_compare(policy, value_type_t<U>{}, to_point(x));
    });
    auto comp = [to_point, &policy](ref lhs, ref rhs) {
        return left_turn(policy, to_point(lhs), to_point(rhs));
    };
    std::sort(first, pivot, comp);
    std::sort(pivot, last, comp);
}

template <class T, class... Ts, disable_int_if_policy<T> = 0>
auto sort_by_angle(T&& x, Ts&&... xs) {
    return sort_by_angle(geometry_policy, x, xs...);
}

} // namespace ac
