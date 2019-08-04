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

template <class Policy, index N, class T0, class T1, class K>
inline auto project(line_scalar_policy<Policy> lsp, const point<T0, N>& p,
                    const line<T1, N, K>& l) {
    auto& policy = lsp.policy;
    return ratio(policy, dot(policy, p - l.start, l.vector), dot(policy, l.vector));
}

template <class Policy, index N, class T0, class T1, class K>
inline auto project(const Policy& policy, const point<T0, N>& p, const line<T1, N, K>& l) {
    return l(project(line_scalar_policy{policy}, p, l));
}

}  // namespace ac
