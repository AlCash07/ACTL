/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/geometry/line.hpp>

namespace ac::detail {

template <class T>
inline bool endpoint_test(uint8_t kind, const T& lhs, const T& rhs = T{0}) {
    switch (kind) {
        case endpoint::free: return true;
        case endpoint::closed: return lhs >= rhs;
        case endpoint::open: return lhs > rhs;
    }
    ACTL_ASSERT(false);
    return false;
}

template <index N, class T0, class K, class T1>
inline bool line_test(const line<T0, N, K>& line, const T1& num, const T1& den) {
    return endpoint_test(line.start_kind(), num) && endpoint_test(line.end_kind(), den, num);
}

}  // namespace ac::detail
