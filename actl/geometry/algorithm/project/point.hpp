/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/geometry/point.hpp>

namespace ac {

template <int N, class T0, class T1>
inline auto project(use_default, const point<N, T0>& src, const point<N, T1>& dst) {
    return dst;
}

}  // namespace ac
