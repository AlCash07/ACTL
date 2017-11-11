/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/geometry/traits/geometry_traits.hpp>
#include <actl/type_traits/type_traits.hpp>

namespace ac { namespace geometry {

// Never change the order of existing IDs! However, new ones can be inserted anywhere.
enum : int {
    scalar_id,
    point_id,
    line_id,
    plane_id,
    sphere_id,

    multi_point_id,
    polygon_id,
    simple_polygon_id,
    star_polygon_id,
    monotone_polygon_id,
    convex_polygon_id,
    convex_monotone_polygon_id
};

template <class T>
struct id : id<tag_t<T>> {};

template <> struct id<scalar_tag> : int_constant<scalar_id> {};
template <> struct id<point_tag>  : int_constant<point_id> {};
template <> struct id<line_tag>   : int_constant<line_id> {};
template <> struct id<plane_tag>  : int_constant<plane_id> {};
template <> struct id<sphere_tag> : int_constant<sphere_id> {};

template <> struct id<multi_point_tag>             : int_constant<multi_point_id> {};
template <> struct id<polygon_tag>                 : int_constant<polygon_id> {};
template <> struct id<simple_polygon_tag>          : int_constant<simple_polygon_id> {};
template <> struct id<star_polygon_tag>            : int_constant<star_polygon_id> {};
template <> struct id<monotone_polygon_tag>        : int_constant<monotone_polygon_id> {};
template <> struct id<convex_polygon_tag>          : int_constant<convex_polygon_id> {};
template <> struct id<convex_monotone_polygon_tag> : int_constant<convex_monotone_polygon_id> {};

template <class T0, class T1, class R = void>
using enable_if_swap_t = std::enable_if_t<id<T1>::value < id<T0>::value, R>;

}}  // namespace ac::geometry
