// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/geometry/algorithm/orientation/point_point.hpp>
#include <actl/geometry/algorithm/within/within.hpp>
#include <actl/geometry/detail/between_endpoints.hpp>

namespace ac {

template <class Policy, index N, class T0, class T1, class K>
enum within within(const Policy& policy, const point<T0, N>& p, const line<T1, N, K>& l) {
    if (degenerate(policy, l))
        return equal(policy, p, l.begin) ? within::border : within::outside;
    if (!collinear(policy, p - l.begin, l.vector))
        return within::outside;
    return detail::between_endpoints(policy, p, l) ? within::border : within::outside;
}

}  // namespace ac
