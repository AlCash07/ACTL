/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/geometry/algorithm/area/point_line.hpp>
#include <actl/geometry/algorithm/distance/point_point.hpp>

namespace ac {

template <class Policy, index N, class T0, class T1, class K>
inline auto distance(Policy&& policy, const point<T0, N>& p, const line<T1, N, K>& l) {
    if (l.start_kind() != endpoint::free && less(policy, dot(policy, p - l.start, l.vector), 0))
        return norm(policy, p - l.start);
    if (l.end_kind() != endpoint::free && less(policy, 0, dot(policy, p - l.end(), l.vector)))
        return norm(policy, p - l.end());
    return ratio(policy, adl::abs(area(policy, p, l)), norm(policy, l.vector));
}

}  // namespace ac
