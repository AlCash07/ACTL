// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/operation/scalar/all.hpp>
#include <actl/utility/index.hpp>
#include <actl/utility/use_default.hpp>

namespace ac {

// clang-format off
struct scalar_tag {};
struct point_tag {};
struct line_tag {};
struct plane_tag {};
struct sphere_tag {};

struct multi_point_tag {};
struct polygon_tag : multi_point_tag {};
struct simple_polygon_tag : polygon_tag {};
struct star_polygon_tag : simple_polygon_tag {};
struct monotone_polygon_tag : simple_polygon_tag {};
struct convex_polygon_tag : star_polygon_tag {};
struct convex_monotone_polygon_tag : monotone_polygon_tag {};
// clang-format on

template<typename T>
struct geometry_traits {
    using tag = scalar_tag;
    using scalar = T;
    static constexpr int dimension = 0;
};

template<typename Tag, typename Point>
struct geometry_traits_base {
    using tag = Tag;
    using scalar = typename geometry_traits<Point>::scalar;
    using point = Point;
    static constexpr int dimension = geometry_traits<Point>::dimension;
};

namespace geometry {

template<typename T>
using tag_t = typename geometry_traits<std::remove_cvref_t<T>>::tag;

template<typename... Ts>
using scalar_t = std::common_type_t<
    typename geometry_traits<std::remove_cvref_t<Ts>>::scalar...>;

template<typename T>
using point_t = typename geometry_traits<std::remove_cvref_t<T>>::point;

/* ID */

// Never change the order of existing IDs! However, new ones can be inserted
// anywhere.
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

template<typename T>
struct id : id<tag_t<T>> {};

// clang-format off
template <> struct id<scalar_tag> : index_constant<scalar_id> {};
template <> struct id<point_tag>  : index_constant<point_id> {};
template <> struct id<line_tag>   : index_constant<line_id> {};
template <> struct id<plane_tag>  : index_constant<plane_id> {};
template <> struct id<sphere_tag> : index_constant<sphere_id> {};

template <> struct id<multi_point_tag>             : index_constant<multi_point_id> {};
template <> struct id<polygon_tag>                 : index_constant<polygon_id> {};
template <> struct id<simple_polygon_tag>          : index_constant<simple_polygon_id> {};
template <> struct id<star_polygon_tag>            : index_constant<star_polygon_id> {};
template <> struct id<monotone_polygon_tag>        : index_constant<monotone_polygon_id> {};
template <> struct id<convex_polygon_tag>          : index_constant<convex_polygon_id> {};
template <> struct id<convex_monotone_polygon_tag> : index_constant<convex_monotone_polygon_id> {};

template<typename T0, typename T1>
concept reverse_order = id<T1>::value < id<T0>;
// clang-format on

} // namespace geometry

cast_before<Div, double> geometry_policy;

} // namespace ac
