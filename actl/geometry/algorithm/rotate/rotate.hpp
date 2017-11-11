/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/geometry/point.hpp>

namespace ac {

template <int N, class T, class... Ts>
inline auto rotate(const point<N, T>& point, const Ts&... args) {
    return rotate(use_default(), point, args...);
}

}  // namespace ac
