// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/numeric/arithmetic/additive/subtract.hpp>

namespace ac {

struct SubtractScalar : operation_base<SubtractScalar> {
    using parent = Subtract::op;

    template<typename L, typename R>
    static constexpr auto evaluate(L l, R r) {
        return l - r;
    }
};
inline constexpr SubtractScalar subtract_scalar;

namespace Subtract {
template<typename L, typename R>
    requires(std::is_arithmetic_v<L> && std::is_arithmetic_v<R>)
constexpr auto specialization(op, type_array<L, R>) noexcept {
    return subtract_scalar;
}
} // namespace Subtract

} // namespace ac
