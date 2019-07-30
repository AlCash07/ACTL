/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/geometry/line.hpp>

namespace ac::detail {

template <class Policy, class T, class U>
inline bool endpoint_test(const Policy& policy, uint8_t kind, const T& lhs, const U& rhs) {
    switch (kind) {
        case endpoint::free: return true;
        case endpoint::closed: return !less(policy, rhs, lhs);
        case endpoint::open: return less(policy, lhs, rhs);
    }
    ACTL_ASSERT(false);
    return false;
}

template <class Policy, index N, class T0, class K, class T1>
inline bool line_test(const Policy& policy, const line<T0, N, K>& l, const T1& num, const T1& den) {
    return endpoint_test(policy, l.start_kind(), 0, num) &&
           endpoint_test(policy, l.end_kind(), num, den);
}

}  // namespace ac::detail
