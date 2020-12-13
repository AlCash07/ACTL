// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/geometry/algorithm/distance/point_point.hpp>
#include <actl/geometry/algorithm/nearest/nearest.hpp>
#include <actl/geometry/multi_point.hpp>
#include <actl/range/algorithm.hpp>
#include <actl/std/vector.hpp>

namespace ac {

namespace detail {

template <class Point, class Policy>
inline auto nearest(const Policy& policy, const span<Point>& points, const span<Point>& tmp) {
    using T = decltype(distance(policy, points[0], points[0]));
    using Pair = std::pair<T, std::pair<Point, Point>>;
    auto y_comp = [&policy](const Point& lhs, const Point& rhs) {
        return less(policy, lhs.y(), rhs.y());
    };
    const index n = points.size();
    if (n <= 3) {
        Pair res;
        for (index i = 0; i != n; ++i) {
            for (index j = i + 1; j != n; ++j) {
                auto dist = distance(policy, points[i], points[j]);
                if (j == 1 || dist < res.first)
                    res = std::pair{dist, std::pair{points[i], points[j]}};
            }
        }
        sort(points, y_comp);
        return res;
    }
    index middle = n / 2;
    auto middle_x = points[middle].x();
    Pair res = nearest(policy, points.first(middle), tmp);
    if (Pair rres = nearest(policy, points.last(n - middle), tmp); rres.first < res.first)
        res = rres;
    merge(points.first(middle), points.last(n - middle), tmp.begin(), y_comp);
    copy(tmp.first(n), points.begin());
    index count = 0;
    for (const auto& p : points) {
        if (less(policy, math::abs(p.x() - middle_x), res.first)) {
            for (index i = count - 1; i >= 0; --i) {
                if (!less(policy, p.y() - tmp[i].y(), res.first)) break;
                auto dist = distance(policy, p, tmp[i]);
                if (less(policy, dist, res.first)) res = std::pair{dist, std::pair{p, tmp[i]}};
            }
            tmp[count++] = p;
        }
    }
    return res;
}

}  // namespace detail

/**
 * Minimum distance between two different points from the set : O(N log N).
 */
template <class Policy, class T,
          enable_int_if<is_multi_point_v<T> && geometry_traits<T>::dimension == 2> = 0>
inline auto nearest(const Policy& policy, T& points) {
    ACTL_ASSERT(points.size() > 1);
    sort(points, math::less(policy));
    for (auto i = points.begin(), j = i + 1; j != points.end(); i = j, ++j) {
        if (equal(policy, *i, *j)) return std::pair{*i, *j};
    }
    using Point = typename geometry_traits<T>::point;
    std::vector<Point> tmp(points.size());
    return detail::nearest<Point>(policy, points, tmp).second;
}

}  // namespace ac
