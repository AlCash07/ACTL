// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/geometry/traits.hpp>

namespace ac {

enum class orientation2d {
    left = -1,     // counter-clockwise
    collinear = 0, // degenerate
    right = 1      // clockwise
};

orientation2d operator-(orientation2d o) {
    return static_cast<orientation2d>(-static_cast<int>(o));
}

enum class orientation { collinear = 0, non_collinear = 1 };

enum orientation operator-(enum orientation o) {
    return o;
}

template<typename L, typename R>
    requires geometry::reverse_order<L, R>
auto orientation(Policy auto const& policy, L const& l, R const& r) {
    return -orientation(policy, r, l);
}

template<typename T, typename... Ts, disable_int_if_policy<T> = 0>
auto orientation(T const& t, Ts const&... ts) {
    return orientation(geometry_policy, t, ts...);
}

template<typename... Ts>
bool left_turn(Ts const&... ts) {
    return orientation(ts...) == orientation2d::left;
}

template<typename... Ts>
bool right_turn(Ts const&... ts) {
    return orientation(ts...) == orientation2d::right;
}

template<typename... Ts>
bool collinear(Ts const&... ts) {
    return static_cast<int>(orientation(ts...)) == 0;
}

} // namespace ac
