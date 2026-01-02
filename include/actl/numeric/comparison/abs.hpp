// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/numeric/comparison/comparison_operation.hpp>
#include <cstdlib>

namespace ac {

struct Abs : operation_base<Abs> {
    using operation_category = comparison_operation;
};
inline constexpr Abs abs;

struct AbsScalar : operation_base<AbsScalar> {
    using parent = Abs;

    template<typename T>
    static constexpr bool requirement = std::is_arithmetic_v<T>;

    template<typename T>
    static constexpr T evaluate(T x) {
        if constexpr (std::is_unsigned_v<T>) {
            return x;
        } else {
            using std::abs;
            return abs(x);
        }
    }
};
AC_REGISTER_OVERLOAD(AbsScalar)
inline constexpr AbsScalar abs_scalar;

} // namespace ac
