// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/numeric/comparison/sgn_scalar.hpp>

namespace ac {

struct Sgn : operation_base<Sgn> {
    using operation_category = comparison_operation;

    template<typename T>
        requires std::is_arithmetic_v<T>
    friend constexpr auto specialization(Sgn, type_array<T>) noexcept {
        return sgn_scalar;
    }
};
inline constexpr Sgn sgn;

} // namespace ac
