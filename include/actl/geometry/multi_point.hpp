/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/geometry/point.hpp>
#include <actl/geometry/traits.hpp>
#include <actl/std/vector.hpp>

namespace ac {

namespace detail {

template <class Range, bool = false>
class multi_point : public Range {
    static_assert(std::is_same_v<point_tag, geometry::tag_t<value_t<Range>>>,
                  "multi_point must be a range of points");

public:
    using Range::Range;
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

namespace detail {

template <class T, class = void>
struct is_polygon : std::false_type {};

template <class T>
struct is_polygon<T, std::void_t<typename T::is_polygon>> : std::true_type {};

template <class T, bool = !is_polygon<T>::value && is_range_v<T>>
struct is_multi_point : std::bool_constant<std::is_same_v<point_tag, geometry::tag_t<value_t<T>>>> {
};

template <class T>
struct is_multi_point<T, false> : std::false_type {};

}  // namespace detail

template <class T>
struct geometry_traits<T, std::enable_if_t<detail::is_multi_point<T>::value>>
    : geometry_traits_base<multi_point_tag, value_t<T>> {};

template <class T>
inline constexpr bool is_multi_point_v = std::is_same_v<multi_point_tag, geometry::tag_t<T>>;

template <class T>
struct identity_functor {
    T operator()(T x) const { return x; }
};

template <class T>
inline identity_functor<reference_t<T>> get_to_point(T&) {
    return {};
}

template <class Indices, class Points>
struct indexed_multi_point {
    static_assert(is_range_v<Indices> && std::is_integral_v<value_t<Indices>> &&
                  std::is_same_v<geometry::tag_t<value_t<Points>>, point_tag>);

    using value_type = value_t<Points>;
    using reference = reference_t<Indices>;

    Indices indices;
    Points points;
    
    auto begin() { return indices.begin(); }
    auto begin() const { return indices.begin(); }

    auto end() { return indices.end(); }
    auto end() const { return indices.end(); }

    friend auto get_to_point(indexed_multi_point& imp) {
        return [&p = imp.points](index x) { return p[static_cast<size_type_t<Points>>(x)]; };
    }
};

template <class I, class P>
indexed_multi_point(I&&, P&&) -> indexed_multi_point<I, P>;

}  // namespace ac
