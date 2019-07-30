/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/geometry/detail/line_test.hpp>

namespace ac::detail {

template <class Policy, index N, class T0, class T1, class K>
inline bool between_endpoints(const Policy& policy, const point<T0, N>& p,
                              const line<T1, N, K>& l) {
    if (l.end_kind() == endpoint::free)
        return endpoint_test(policy, l.start_kind(), 0, dot(policy, p - l.start, l.vector));
    if (l.kind() == line_kind::half_open_segment && equal(policy, p, l.start)) return false;
    return endpoint_test(policy, l.end_kind(), 0, dot(policy, l.start - p, p - l.end()));
}

}  // namespace ac::detail
