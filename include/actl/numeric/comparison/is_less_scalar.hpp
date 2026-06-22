// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/numeric/comparison/comparison_operation.hpp>

namespace ac {

struct IsLess;

struct IsLessScalar : operation_base<IsLessScalar> {
    using parent = IsLess;

    template<typename L, typename R>
    static constexpr bool evaluate(L l, R r) {
        return l < r;
    }
};
inline constexpr IsLessScalar is_less_scalar;

} // namespace ac
