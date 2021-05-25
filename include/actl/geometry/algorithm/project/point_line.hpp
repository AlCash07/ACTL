// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/geometry/algorithm/project/project.hpp>
#include <actl/geometry/line.hpp>

namespace ac {

template <class Policy, index N, class T0, class T1, class K>
auto project(
    line_scalar_policy<Policy> lsp,
    const point<T0, N>& p,
    const line<T1, N, K>& l)
{
    auto& policy = lsp.policy;
    return ratio(
        policy, dot(policy, p - l.begin, l.vector), dot(policy, l.vector));
}

template <class Policy, index N, class T0, class T1, class K>
auto project(
    const Policy& policy, const point<T0, N>& p, const line<T1, N, K>& l)
{
    return l(project(line_scalar_policy{policy}, p, l));
}

} // namespace ac
