// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/numeric/arithmetic/additive/negate.hpp>

namespace ac {

struct NegateScalar : operation_base<NegateScalar> {
    using parent = Negate::op;

    template<typename T>
    static constexpr auto evaluate(T t) {
        return -t;
    }
};
inline constexpr NegateScalar negate_scalar;

namespace Negate {
template<typename T>
    requires std::is_arithmetic_v<T>
constexpr auto specialization(op, type_array<T>) noexcept {
    return negate_scalar;
}
} // namespace Negate

} // namespace ac
