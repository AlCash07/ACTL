// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/numeric/comparison/equality.hpp>
#include <actl/numeric/logic/logical_not_scalar.hpp>

namespace ac {

struct IsEqualScalar : operation_base<IsEqualScalar> {
    using parent = IsEqual::op;

    static constexpr bool is_commutative = true;

    template<typename L, typename R>
    static constexpr bool evaluate(L l, R r) {
        return l == r;
    }
};
inline constexpr IsEqualScalar is_equal_scalar;

namespace IsEqual {
template<typename L, typename R>
    requires(std::is_arithmetic_v<L> && std::is_arithmetic_v<R>)
constexpr auto specialization(op, type_array<L, R>) noexcept {
    return is_equal_scalar;
}
} // namespace IsEqual

} // namespace ac
