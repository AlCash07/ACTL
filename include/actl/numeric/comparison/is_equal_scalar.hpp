// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/numeric/comparison/comparison_operation.hpp>

namespace ac {

struct IsEqual;

struct IsEqualScalar : operation_base<IsEqualScalar> {
    using parent = IsEqual;

    static constexpr bool is_commutative = true;

    template<typename L, typename R>
    static constexpr bool evaluate(L l, R r) {
        return l == r;
    }
};
inline constexpr IsEqualScalar is_equal_scalar;

} // namespace ac
