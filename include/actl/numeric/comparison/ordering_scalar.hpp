// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/numeric/comparison/ordering.hpp>
#include <actl/numeric/logic/logical_not_scalar.hpp>

namespace ac {

struct IsLessScalar : operation_base<IsLessScalar> {
    using parent = IsLess::op;

    template<typename L, typename R>
    static constexpr bool evaluate(L l, R r) {
        return l < r;
    }
};
inline constexpr IsLessScalar is_less_scalar;

namespace IsLess {
template<typename L, typename R>
    requires(std::is_arithmetic_v<L> && std::is_arithmetic_v<R>)
constexpr auto specialization(op, type_array<L, R>) noexcept {
    return is_less_scalar;
}
} // namespace IsLess

} // namespace ac
