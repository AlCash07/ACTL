// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/geometry/point.hpp>
#include <actl/iterator/cyclic_iterator.hpp>
#include <actl/range/algorithm.hpp>
#include <actl/range/traits/dependent.hpp>
#include <actl/std/vector.hpp>

namespace ac {

namespace detail {

template <class Range, bool = std::is_same_v<geometry::tag_t<Range>, point_tag>>
class polygon : public Range
{
    static_assert(
        std::is_same_v<point_tag, geometry::tag_t<range_value_t<Range>>>,
        "polygon must be a range of points");

public:
    struct is_polygon;

    using Range::Range;
};

template <class Point>
class polygon<Point, true> : public polygon<std::vector<Point>>
{
public:
    using polygon<std::vector<Point>>::polygon;
};

} // namespace detail

/// Polygon - closed polyline defined by the sequence of vertices, that all lie
/// in the same plane. Algorithms usually expect vertices to go in
/// counter-clockwise order.
/// @tparam T either a point (then std::vector is used as container) or a range
/// of points.
template <class T>
class polygon : public detail::polygon<T>
{
public:
    using detail::polygon<T>::polygon;
};

template <class T>
struct geometry_traits<polygon<T>>
    : geometry_traits_base<polygon_tag, range_value_t<polygon<T>>>
{};

/// Simple polygon - the boundary doesn't cross itself.
template <class T>
class simple_polygon : public polygon<T>
{
public:
    using polygon<T>::polygon;
};

template <class T>
struct geometry_traits<simple_polygon<T>>
    : geometry_traits_base<simple_polygon_tag, range_value_t<polygon<T>>>
{};

/// Star polygon - has observer point, from which all the boundary is visible.
template <class T>
class star_polygon : public simple_polygon<T>
{
public:
    using point_type = range_value_t<simple_polygon<T>>;

    point_type const& observer() const
    {
        return observer_;
    }

    point_type observer_;
};

template <class T>
struct geometry_traits<star_polygon<T>>
    : geometry_traits_base<star_polygon_tag, range_value_t<polygon<T>>>
{};

/// Specific monotone polygon - with monotony direction (1, 0).
/// Bottom-left vertex has index 0, top-right index is returned by right().
template <class T>
class monotone_polygon : public simple_polygon<T>
{
public:
    index right() const
    {
        return right_;
    }

    void right(index value)
    {
        right_ = value;
    }

    index right_;
};

template <class T>
struct geometry_traits<monotone_polygon<T>>
    : geometry_traits_base<monotone_polygon_tag, range_value_t<polygon<T>>>
{};

/// Convex polygon.
template <class T>
class convex_polygon : public simple_polygon<T>
{
public:
    using simple_polygon<T>::simple_polygon;

    range_reference_t<simple_polygon<T> const> observer() const
    {
        return *this->begin();
    }
};

template <class T>
struct geometry_traits<convex_polygon<T>>
    : geometry_traits_base<convex_polygon_tag, range_value_t<polygon<T>>>
{};

/// Theoretically, every convex polygon is monotone. However, our definition of
/// monotone polygon requires special properties.
template <class T>
class convex_monotone_polygon : public monotone_polygon<T>
{
public:
    using monotone_polygon<T>::monotone_polygon;

    template <class T1>
    convex_monotone_polygon(convex_polygon<T1> const& polygon)
    {
        this->resize(polygon.size());
        auto minmax = minmax_element(polygon);
        std::rotate_copy(
            polygon.begin(), minmax.first, polygon.end(), this->begin());
        this->right_ = minmax.second - minmax.first;
        if (this->right_ < 0)
            this->right_ += static_cast<index>(polygon.size());
    }

    range_reference_t<monotone_polygon<T> const> observer() const
    {
        return *this->begin();
    }
};

template <class T>
struct geometry_traits<convex_monotone_polygon<T>>
    : geometry_traits_base<
          convex_monotone_polygon_tag,
          range_value_t<polygon<T>>>
{};

} // namespace ac
