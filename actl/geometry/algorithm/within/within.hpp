/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/geometry/point.hpp>

namespace ac {

/**
 * Point inclusion test, returns -1 if inside, 0 if on the border, 1 if outside.
 */
template <int N, class T0, class T1>
inline int within(const point<T0, N>& lhs, const T1& rhs) {
    return within(use_default(), lhs, rhs);
}

}  // namespace ac
