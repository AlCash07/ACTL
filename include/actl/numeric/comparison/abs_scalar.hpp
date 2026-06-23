// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/numeric/comparison/abs.hpp>
#include <cstdlib>

namespace ac {

struct AbsScalar : operation_base<AbsScalar> {
    using parent = Abs::op;

    template<typename T>
    static constexpr T evaluate(T t) {
        if constexpr (std::is_unsigned_v<T>) {
            return t;
        } else {
            using std::abs;
            return abs(t);
        }
    }
};
inline constexpr AbsScalar abs_scalar;

namespace Abs {
template<typename T>
    requires std::is_arithmetic_v<T>
constexpr auto specialization(op, type_array<T>) noexcept {
    return abs_scalar;
}
} // namespace Abs

} // namespace ac
