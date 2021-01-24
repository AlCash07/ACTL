// Copyright 2019 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/geometry/algorithm/orientation/orientation.hpp>
#include <actl/geometry/algorithm/within/within.hpp>

namespace ac::detail {

enum within to_inclusion(orientation2d x) {
    return static_cast<enum within>(1 - static_cast<int>(x) + static_cast<int>(within::inside));
}

} // namespace ac::detail
