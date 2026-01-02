// Copyright 2019 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <concepts>

namespace ac {

struct Pi {
    template<std::floating_point Float>
    explicit constexpr operator Float() const noexcept {
        return static_cast<Float>(3.141592653589793238462643383279502884L);
    }
};
inline constexpr Pi pi;

} // namespace ac
