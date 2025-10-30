// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/geometry/algorithm/point/norm.hpp>

namespace ac {

template<index N, typename TL, typename TR>
auto bisector(
    Policy auto const& policy, point<TL, N> const& l, point<TR, N> const& r
) {
    return product(policy, l, norm(policy, r)) +
           product(policy, r, norm(policy, l));
}

template<index N, typename TL, typename TR>
auto bisector(point<TL, N> const& l, point<TR, N> const& r) {
    return bisector(geometry_policy, l, r);
}

} // namespace ac
