/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/functions.hpp>
#include <actl/geometry/algorithm/distance/point_point.hpp>
#include <actl/geometry/multi_point.hpp>
#include <actl/range/algorithm.hpp>
#include <utility>
#include <vector>

namespace ac {

template <class DistancePolicy>
struct nearest_multi_point : DistancePolicy {};

template <class P = use_default>
using comparable_nearest_multi_point = nearest_multi_point<comparable_distance_point_point<P>>;

template <class P = use_default, class S = use_default>
using standard_nearest_multi_point = nearest_multi_point<standard_distance_point_point<P, S>>;

namespace detail {

template <class DP, class Iterator, class Point>
inline auto nearest(const nearest_multi_point<DP>& policy, Iterator first, Iterator last,
                    std::vector<Point>& tmp) {
    using return_t = std::pair<decltype(distance(policy, *first, *first)), std::pair<Point, Point>>;
    auto y_cmp = [](const Point& lhs, const Point& rhs) { return lhs[1] < rhs[1]; };
    if (last - first <= 3) {
        return_t res;
        for (auto i = first; i != last; ++i) {
            for (auto j = i + 1; j != last; ++j) {
                auto dist = distance(policy, *i, *j);
                if (j == first + 1 || dist < res.first)
                    res = std::pair(dist, std::pair(*i, *j));
            }
        }
        std::sort(first, last, y_cmp);
        return res;
    }
    auto middle = first + (last - first) / 2;
    auto middle_x = (*middle)[0];
    return_t res  = nearest(policy, first, middle, tmp);
    return_t res1 = nearest(policy, middle, last, tmp);
    if (res1.first < res.first) res = res1;
    tmp.resize(last - first);
    std::merge(first, middle, middle, last, tmp.begin(), y_cmp);
    std::copy(tmp.begin(), tmp.end(), first);
    tmp.clear();
    for (middle = first; middle != last; ++middle) {
        if (abs((*middle)[0] - middle_x) < res.first) {
            for (auto i = tmp.rbegin(); i != tmp.rend(); ++i) {
                if ((*middle)[1] - (*i)[1] >= res.first) break;
                auto dist = distance(policy, *middle, *i);
                if (dist < res.first) res = std::pair(dist, std::pair(*middle, *i));
            }
            tmp.push_back(*middle);
        }
    }
    return res;
}

}  // namespace detail

/**
 * Minimum distance between two different points from the set : O(N log N).
 */
template <class DP, class T,
          class = std::enable_if_t<geometry_traits<multi_point<T>>::dimension == 2>>
inline auto nearest(const nearest_multi_point<DP>& policy, multi_point<T>& points) {
    ACTL_ASSERT(points.size() > 1);
    sort(points);
    for (auto it = points.begin(); it + 1 != points.end(); ++it) {
        if (it[0] == it[1]) return std::pair(it[0], it[1]);
    }
    std::vector<typename geometry_traits<multi_point<T>>::point> tmp(points.size());
    return detail::nearest(policy, points.begin(), points.end(), tmp).second;
}

template <class T, class = std::enable_if_t<geometry_traits<multi_point<T>>::dimension == 2>>
inline auto nearest(use_default, multi_point<T>& points) {
    return nearest(comparable_nearest_multi_point<>(), points);
}

}  // namespace ac
