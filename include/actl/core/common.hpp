// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/AC_ASSERT.hpp>
#include <actl/core/scalar_operation.hpp>
#include <actl/utility/none.hpp>

namespace ac {

struct Common : operation_base<Common> {
    using operation_category = scalar_operation;

    static constexpr bool is_associative = true;
    static constexpr bool is_commutative = true;

    template<typename T>
    static constexpr T evaluate(T x) {
        return x;
    }

    template<typename T>
    static constexpr T evaluate(T x, T y) {
        AC_ASSERT(x == y);
        return x;
    }

    template<typename T0, typename T1, typename T2, typename... Ts>
    static constexpr auto evaluate(T0 x0, T1 x1, T2 x2, Ts... xs) {
        return evaluate(evaluate(x0, x1), x2, xs...);
    }
};
inline constexpr Common common;

template<typename T>
struct identity_element<Common, T> {
    static constexpr none value() {
        return none{};
    }
};

} // namespace ac
