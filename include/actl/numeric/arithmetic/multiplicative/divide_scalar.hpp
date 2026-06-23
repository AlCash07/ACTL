// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/numeric/arithmetic/multiplicative/divide.hpp>

namespace ac {

struct DivideScalar : operation_base<DivideScalar> {
    using parent = Divide::op;

    template<typename L, typename R>
    static constexpr auto evaluate(L l, R r) {
        return l / r;
    }
};
inline constexpr DivideScalar divide_scalar;

namespace Divide {
template<typename L, typename R>
    requires(std::is_arithmetic_v<L> && std::is_arithmetic_v<R>)
constexpr auto specialization(op, type_array<L, R>) noexcept {
    return divide_scalar;
}
} // namespace Divide

} // namespace ac
