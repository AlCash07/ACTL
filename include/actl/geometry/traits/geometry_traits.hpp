/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/traits/type_traits.hpp>

namespace ac {

struct scalar_tag {};
struct point_tag {};
struct line_tag {};
struct plane_tag {};
struct sphere_tag {};

struct multi_point_tag {};
struct polygon_tag {};
struct simple_polygon_tag {};
struct star_polygon_tag {};
struct monotone_polygon_tag {};
struct convex_polygon_tag {};
struct convex_monotone_polygon_tag {};

template <class T>
struct geometry_traits {
    using tag    = scalar_tag;
    using scalar = T;
    static constexpr int dimension = 0;
};

template <class Tag, class Point>
struct geometry_traits_base {
    using tag    = Tag;
    using scalar = typename geometry_traits<Point>::scalar;
    using point  = Point;
    static constexpr int dimension = geometry_traits<Point>::dimension;
};

namespace geometry {

template <class T>
struct base_tag {
    using type = T;
};

template <> struct base_tag<polygon_tag>                 { using type = multi_point_tag;      };
template <> struct base_tag<simple_polygon_tag>          { using type = polygon_tag;          };
template <> struct base_tag<star_polygon_tag>            { using type = simple_polygon_tag;   };
template <> struct base_tag<monotone_polygon_tag>        { using type = simple_polygon_tag;   };
template <> struct base_tag<convex_polygon_tag>          { using type = star_polygon_tag;     };
template <> struct base_tag<convex_monotone_polygon_tag> { using type = monotone_polygon_tag; };

template <class T>
using base_tag_t = typename base_tag<T>::type;

template <class T>
using tag_t = typename geometry_traits<remove_cvref_t<T>>::tag;

template <class... Ts>
using scalar_t = std::common_type_t<typename geometry_traits<remove_cvref_t<Ts>>::scalar...>;

}  // namespace geometry

}  // namespace ac
