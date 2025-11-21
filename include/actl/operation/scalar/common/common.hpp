// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/AC_ASSERT.hpp>
#include <actl/operation/scalar/scalar_operation.hpp>
#include <actl/utility/none.hpp>

namespace ac {

struct common_f : scalar_operation<common_f, 2> {
    using operation_category = scalar_operation_tag;

    static constexpr bool is_associative = true;
    static constexpr bool is_commutative = true;

    template<typename... Ts>
    constexpr auto evaluate(Ts const&... xs) const {
        return eval_scalar(eval(xs)...);
    }

    template<typename T>
    static constexpr T eval_scalar(T x) {
        return x;
    }

    template<typename T>
    static constexpr T eval_scalar(T x, T y) {
        AC_ASSERT(x == y);
        return x;
    }

    template<typename T0, typename T1, typename T2, typename... Ts>
    static constexpr auto eval_scalar(T0 x0, T1 x1, T2 x2, Ts... xs) {
        return eval_scalar(eval_scalar(x0, x1), x2, xs...);
    }
};
inline constexpr common_f common;

template<typename T>
struct identity_element<common_f, T> {
    static constexpr none value() {
        return none{};
    }
};

} // namespace ac
