// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/geometry/line.hpp>

namespace ac::detail {

template<typename L, typename R>
bool endpoint_test(
    Policy auto const& policy, endpoint kind, L const& l, R const& r
) {
    switch (kind) {
        case endpoint::free:
            return true;
        case endpoint::closed:
            return !less(policy, r, l);
        case endpoint::open:
            return less(policy, l, r);
    }
}

template<typename T, typename U>
bool line_test(
    Policy auto const& policy, line_kind kind, T const& num, U const& den
) {
    return endpoint_test(policy, begin(kind), 0, num) &&
           endpoint_test(policy, end(kind), num, den);
}

} // namespace ac::detail
