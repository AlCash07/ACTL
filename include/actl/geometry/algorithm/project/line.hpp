/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/geometry/algorithm/project/project.hpp>
#include <actl/geometry/line.hpp>

namespace ac {

template <class P = use_default, class F = use_default>
struct project_line : geometry::policy {};

template <class P = use_default, class F = use_default>
struct project_line_scalar : geometry::policy {};

template <class LS, index N, class T0, class T1, class K, enable_int_if<is_line_scalar_v<LS>> = 0>
inline auto project(LS&& ls, const point<T0, N>& p, const line<T1, N, K>& l) {
    return ratio(ls.policy, dot(ls.policy, p - l.start, l.vector), dot(ls.policy, l.vector));
}

template <class Policy, index N, class T0, class T1, class K,
          enable_int_if<!is_line_scalar_v<Policy>> = 0>
inline auto project(Policy&& policy, const point<T0, N>& p, const line<T1, N, K>& l) {
    return l(project(line_scalar{policy}, p, l));
}

}  // namespace ac
