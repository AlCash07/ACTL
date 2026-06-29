// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/AC_ASSERT.hpp>
#include <actl/core/none.hpp>
#include <actl/operation/operation_base.hpp>

namespace ac {

struct Common : operation_base<Common> {
    static constexpr bool is_associative = true;
    static constexpr bool is_commutative = true;

    static constexpr auto identity_element = none{};

    template<typename T>
    static constexpr T evaluate(T t) {
        return t;
    }

    template<typename T>
    static constexpr T evaluate(T l, T r) {
        AC_ASSERT(l == r);
        return l;
    }

    template<typename T0, typename T1, typename T2, typename... Ts>
    static constexpr auto evaluate(T0 t0, T1 t1, T2 t2, Ts... ts) {
        return evaluate(evaluate(t0, t1), t2, ts...);
    }
};
inline constexpr Common common;

} // namespace ac
