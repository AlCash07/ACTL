// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/geometry/algorithm/within/within.hpp>
#include <actl/geometry/plane.hpp>

namespace ac {

template<index N, class T0, class T1>
enum within within(
    Policy auto const& policy, point<T0, N> const& p, plane<T1, N> const& pl
) {
    return equal(policy, pl(policy, p), 0) ? within::border : within::outside;
}

} // namespace ac
