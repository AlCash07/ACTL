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
template<typename U, typename T>
    requires(is_multi_point_v<U> && geometry_traits<U>::dimension == 2)
void sort_by_angle(
    Policy auto const& policy, U& points, point<T> const& origin
) {
    using ref = range_reference_t<U>;
    auto to_point = get_to_point(points);
    auto first = points.begin(), last = points.end();
    first = std::partition(first, last, [to_point, &policy, &origin](ref x) {
        return equal(policy, origin, to_point(x));
    });
    auto pivot =
        std::partition(first, last, [to_point, &policy, &origin](ref x) {
            return y_compare(policy, origin, to_point(x));
        });
    auto comp = [to_point, &policy](ref l, ref r) {
        return left_turn(policy, to_point(l), to_point(r));
    };
    std::sort(first, pivot, comp);
    std::sort(pivot, last, comp);
}

/// Sort by angle around (0, 0).
template<typename U>
    requires(is_multi_point_v<U> && geometry_traits<U>::dimension == 2)
void sort_by_angle(Policy auto const& policy, U& points) {
    using ref = range_reference_t<U>;
    auto to_point = get_to_point(points);
    auto first = points.begin(), last = points.end();
    first = std::partition(first, last, [to_point, &policy](ref x) {
        return degenerate(policy, to_point(x));
    });
    auto pivot = std::partition(first, last, [to_point, &policy](ref x) {
        return y_compare(policy, range_value_t<U>{}, to_point(x));
    });
    auto comp = [to_point, &policy](ref l, ref r) {
        return left_turn(policy, to_point(l), to_point(r));
    };
    std::sort(first, pivot, comp);
    std::sort(pivot, last, comp);
}

template<typename T, typename... Ts, disable_int_if_policy<T> = 0>
auto sort_by_angle(T&& x, Ts&&... xs) {
    return sort_by_angle(geometry_policy, x, xs...);
}

} // namespace ac
