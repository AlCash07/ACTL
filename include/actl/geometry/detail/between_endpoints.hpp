// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/geometry/detail/line_test.hpp>

namespace ac::detail {

template<index N, typename T0, typename T1, typename K>
bool between_endpoints(
    Policy auto const& policy, point<T0, N> const& p, line<T1, N, K> const& l
) {
    if (end(l.kind()) == endpoint::free)
        return endpoint_test(
            policy, begin(l.kind()), 0, dot(policy, p - l.begin, l.vector)
        );
    if (l.kind() == line_kind::half_open_segment && equal(policy, p, l.begin))
        return false;
    return endpoint_test(
        policy, end(l.kind()), 0, dot(policy, l.begin - p, p - l.end())
    );
}

} // namespace ac::detail
