// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/numeric/arithmetic/arithmetic_operation.hpp>

namespace ac {

struct Multiply;

struct MultiplyInteger : operation_base<MultiplyInteger> {
    using parent = Multiply;

    static constexpr bool is_associative = true;
    static constexpr bool is_commutative = true;

    template<typename L, typename R>
    static constexpr auto evaluate(L l, R r) {
        return l * r;
    }
};
inline constexpr MultiplyInteger multiply_integer;

struct MultiplyFloat : operation_base<MultiplyFloat> {
    using parent = Multiply;

    // floating point multiplication is neither associative nor commutative

    template<typename L, typename R>
    static constexpr auto evaluate(L l, R r) {
        return l * r;
    }
};
inline constexpr MultiplyFloat multiply_float;

} // namespace ac
