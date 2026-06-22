// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/numeric/arithmetic/arithmetic_operation.hpp>

namespace ac {

struct Divide;

struct DivideScalar : operation_base<DivideScalar> {
    using parent = Divide;

    template<typename L, typename R>
    static constexpr auto evaluate(L l, R r) {
        return l / r;
    }
};
inline constexpr DivideScalar divide_scalar;

} // namespace ac
