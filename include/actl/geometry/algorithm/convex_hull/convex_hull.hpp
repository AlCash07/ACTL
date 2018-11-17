/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/geometry/multi_point.hpp>
#include <actl/util/use_default.hpp>

namespace ac {

template <class T>
inline auto convex_hull(multi_point<T>& points) {
    return convex_hull(use_default{}, points);
}

}  // namespace ac
