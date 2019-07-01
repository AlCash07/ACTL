/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/geometry/multi_point.hpp>
#include <actl/iterator/cyclic_iterator.hpp>
#include <actl/range/algorithm.hpp>

namespace ac {

/**
 * Polygon - closed polyline defined by the sequence of vertices, that all lie in the same plane.
 * Algorithms usually expect vertices to go in counter-clockwise order.
 */
template <class T>
class polygon : public multi_point<T> {
public:
    using cyclic_iterator       = ac::cyclic_iterator<iterator_t<multi_point<T>>>;
    using const_cyclic_iterator = ac::cyclic_iterator<iterator_t<const multi_point<T>>>;

    using multi_point<T>::multi_point;

    auto cyclic(iterator_t<multi_point<T>> it) {
        return cyclic_iterator{it, this->begin(), this->end()};
    }

    auto cyclic(iterator_t<const multi_point<T>> it) const {
        return const_cyclic_iterator{it, this->begin(), this->end()};
    }

    auto cyclic_begin() { return cyclic(this->begin()); }
    auto cyclic_begin() const { return cyclic(this->begin()); }
};

template <class T>
struct geometry_traits<polygon<T>> : geometry_traits_base<polygon_tag, value_t<polygon<T>>> {};

/**
 * Simple polygon - the boundary doesn't cross itself.
 */
template <class T>
class simple_polygon : public polygon<T> {
public:
    using polygon<T>::polygon;
};

template <class T>
struct geometry_traits<simple_polygon<T>>
    : geometry_traits_base<simple_polygon_tag, value_t<polygon<T>>> {};

/**
 * Star polygon - has observer point, from which all the boundary is visible.
 */
template <class T>
class star_polygon : public polygon<T> {
public:
    using point_type = value_t<polygon<T>>;

    using polygon<T>::polygon;

    const point_type& observer() const { return observer_; }

    point_type observer_;
};

template <class T>
struct geometry_traits<star_polygon<T>>
    : geometry_traits_base<star_polygon_tag, value_t<polygon<T>>> {};

/**
 * Specific monotone polygon - with monotony direction (1, 0).
 * Bottom-left vertex has index 0, top-right index is returned by right().
 */
template <class T>
class monotone_polygon : public polygon<T> {
public:
    using polygon<T>::polygon;

    int right() const { return right_; }

    int right_;
};

template <class T>
struct geometry_traits<monotone_polygon<T>>
    : geometry_traits_base<monotone_polygon_tag, value_t<polygon<T>>> {};

/**
 * Convex polygon.
 */
template <class T>
class convex_polygon : public polygon<T> {
public:
    using polygon<T>::polygon;

    const reference_t<polygon<T>> observer() const { return *this->begin(); }
};

template <class T>
struct geometry_traits<convex_polygon<T>>
    : geometry_traits_base<convex_polygon_tag, value_t<polygon<T>>> {};

/**
 * Theoretically, every convex polygon is monotone. However, our definition of monotone polygon
 * requires special properties.
 */
template <class T>
class convex_monotone_polygon : public monotone_polygon<T> {
public:
    using monotone_polygon<T>::monotone_polygon;

    template <class T1>
    convex_monotone_polygon(const convex_polygon<T1>& polygon) {
        this->resize(polygon.size());
        auto minmax = minmax_element(polygon);
        std::rotate_copy(polygon.begin(), minmax.first, polygon.end(), this->begin());
        this->right_ = static_cast<int>(minmax.second - minmax.first);
        if (this->right_ < 0) this->right_ += static_cast<int>(polygon.size());
    }

    const reference_t<monotone_polygon<T>> observer() const { return *this->begin(); }
};

template <class T>
struct geometry_traits<convex_monotone_polygon<T>>
    : geometry_traits_base<convex_monotone_polygon_tag, value_t<polygon<T>>> {};

}  // namespace ac
