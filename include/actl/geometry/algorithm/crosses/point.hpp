// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/geometry/algorithm/crosses/crosses.hpp>
#include <actl/geometry/algorithm/within/within.hpp>
#include <actl/geometry/point.hpp>

namespace ac {

template<index N, typename TL, typename R>
bool crosses(Policy auto const& policy, point<TL, N> const& l, R const& r) {
    return within(policy, l, r) == within::border;
}

} // namespace ac
