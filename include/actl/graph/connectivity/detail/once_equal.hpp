/***************************************************************************************************
 * Copyright 2019 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/graph/traversal/depth_first_search.hpp>

namespace ac::detail {

template <bool Once, bool Other>
struct once_equal {
    operator bool() {
        if (x_ == Other) return Other;
        x_ = Other;
        return Once;
    }

    bool x_ = true;
};

template <bool All>
struct once_equal<All, All> {
    constexpr operator bool() { return All; }
};

}  // namespace ac::detail
