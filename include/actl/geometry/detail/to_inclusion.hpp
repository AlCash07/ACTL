/***************************************************************************************************
 * Copyright 2019 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/geometry/algorithm/within/within.hpp>

namespace ac::detail {

inline enum within to_inclusion(int orientation) {
    return static_cast<enum within>(1 - orientation + static_cast<int>(within::inside));
}

}  // namespace ac::detail
