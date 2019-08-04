/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/geometry/algorithm/nearest/nearest.hpp>
#include <actl/geometry/algorithm/project/point_line.hpp>

namespace ac {

template <class Policy, index N, class T0, class T1, class K>
inline auto nearest(const Policy& policy, const point<T0, N>& p, const line<T1, N, K>& l) {
    using Pair = std::pair<const point<T0, N>&, decltype(project(policy, p, l))>;
    if (l.start_kind() != endpoint::free && !less(policy, 0, dot(policy, p - l.start, l.vector)))
        return Pair{p, l.start};
    if (l.end_kind() != endpoint::free && !less(policy, dot(policy, p - l.end(), l.vector), 0))
        return Pair{p, l.end()};
    return Pair{p, project(policy, p, l)};
}

}  // namespace ac
