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

orientation2d operator-(orientation2d x) {
    return static_cast<orientation2d>(-static_cast<int>(x));
}

enum class orientation { collinear = 0, non_collinear = 1 };

enum orientation operator-(enum orientation x) {
    return x;
}

template<class T, class U>
    requires geometry::reverse_order<T, U>
auto orientation(Policy auto const& policy, T const& lhs, U const& rhs) {
    return -orientation(policy, rhs, lhs);
}

template<class T, class... Ts, disable_int_if_policy<T> = 0>
auto orientation(T const& x, Ts const&... xs) {
    return orientation(geometry_policy, x, xs...);
}

template<class... Ts>
bool left_turn(Ts const&... xs) {
    return orientation(xs...) == orientation2d::left;
}

template<class... Ts>
bool right_turn(Ts const&... xs) {
    return orientation(xs...) == orientation2d::right;
}

template<class... Ts>
bool collinear(Ts const&... xs) {
    return static_cast<int>(orientation(xs...)) == 0;
}

} // namespace ac
