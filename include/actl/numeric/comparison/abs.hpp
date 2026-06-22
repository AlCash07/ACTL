// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/numeric/comparison/abs_scalar.hpp>

namespace ac {

struct Abs : operation_base<Abs> {
    using operation_category = comparison_operation;

    template<typename T>
        requires std::is_arithmetic_v<T>
    friend constexpr auto specialization(Abs, type_array<T>) noexcept {
        return abs_scalar;
    }
};
inline constexpr Abs abs;

} // namespace ac
