/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/geometry/2d/point2d.hpp>
#include <actl/geometry/sphere.hpp>

namespace ac {

template <class T>
using circle = sphere<T, 2>;

template <class T0, class T1>
inline constexpr auto make_circle(const point<T0>& center, const T1& radius) {
    return circle<geometry::scalar_t<T0, T1>>{center, radius};
}

}  // namespace ac
