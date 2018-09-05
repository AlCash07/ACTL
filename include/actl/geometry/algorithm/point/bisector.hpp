/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/geometry/algorithm/point/norm.hpp>

namespace ac {

template <class NormPolicy>
struct bisector_policy : NormPolicy {};

template <class P = use_default, class S = use_default>
using standard_bisector = bisector_policy<standard_norm<P, S>>;

template <class NP, int N, class T0, class T1>
inline auto bisector(const bisector_policy<NP>& policy, const point<T0, N>& lhs,
                     const point<T1, N>& rhs) {
    return lhs * norm(policy, rhs) + rhs * norm(policy, lhs);
}

template <int N, class T0, class T1>
inline auto bisector(const point<T0, N>& lhs, const point<T1, N>& rhs) {
    return bisector(standard_bisector<>(), lhs, rhs);
}

}  // namespace ac