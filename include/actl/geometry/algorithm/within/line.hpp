/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/geometry/algorithm/point/collinear.hpp>
#include <actl/geometry/detail/between_endpoints.hpp>

namespace ac {

template <class P = use_default, class CollinearPolicy = comparable_collinear<P>>
struct within_line : CollinearPolicy {};

template <class P, class CP, int N, class T0, class T1, class K>
inline int within(const within_line<P, CP>& policy, const point<T0, N>& point,
                  const line<T1, N, K>& line) {
    if (!line) return point == line.start ? 0 : 1;
    if (!collinear(policy, point - line.start, line.slope)) return 1;
    return detail::between_endpoints<P>(point, line) ? 0 : 1;
}

template <int N, class T0, class T1, class K>
inline int within(use_default, const point<T0, N>& point, const line<T1, N, K>& line) {
    return within(within_line<>(), point, line);
}

}  // namespace ac
