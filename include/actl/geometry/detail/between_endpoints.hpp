/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/geometry/detail/line_test.hpp>

namespace ac::detail {

template <class P, int N, class T0, class T1, class K>
inline bool between_endpoints(const point<T0, N>& point, const line<T1, N, K>& line) {
    if (line.end_kind() == endpoint::free)
        return endpoint_test(line.start_kind(), dot<P>(point - line.start, line.slope));
    if (line.kind == line_kind::open_closed_segment && point == line.start) return false;
    return endpoint_test(line.end_kind(), dot<P>(line.start - point, point - line.end()));
}

}  // namespace ac::detail
