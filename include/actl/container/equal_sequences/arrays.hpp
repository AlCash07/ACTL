// Copyright 2022 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

namespace ac {

// TODO: remove when std::array operator== is constexpr.
template<typename Array>
constexpr bool equal_arrays(Array const& lhs, Array const& rhs) noexcept {
    for (size_t i = 0; i != lhs.size(); ++i)
        if (lhs[i] != rhs[i])
            return false;
    return true;
}

} // namespace ac
