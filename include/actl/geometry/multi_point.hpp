/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/geometry/traits/geometry_traits.hpp>
#include <actl/std/vector.hpp>

namespace ac {

namespace detail {

template <class Range, bool = false>
class multi_point : public Range {
    static_assert(std::is_same_v<point_tag, geometry::tag_t<value_t<Range>>>,
                  "multi_point must be a range of points");

public:
    using Range::Range;

    constexpr int size() const { return static_cast<int>(Range::size()); }
};

template <class Point>
class multi_point<Point, true> : public multi_point<std::vector<Point>> {
public:
    using multi_point<std::vector<Point>>::multi_point;
};

}  // namespace detail

/**
 * Multi-point, a range of points.
 * @tparam T either a point (then std::vector is used as container) or a range of points.
 */
template <class T>
using multi_point = detail::multi_point<T, std::is_same_v<geometry::tag_t<T>, point_tag>>;

template <class T>
struct geometry_traits<multi_point<T>>
    : geometry_traits_base<multi_point_tag, value_t<multi_point<T>>> {};

}  // namespace ac
