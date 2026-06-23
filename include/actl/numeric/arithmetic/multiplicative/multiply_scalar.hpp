// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/numeric/arithmetic/multiplicative/multiply.hpp>

namespace ac {

struct MultiplyInteger : operation_base<MultiplyInteger> {
    using parent = Multiply::op;

    static constexpr bool is_associative = true;
    static constexpr bool is_commutative = true;

    template<typename L, typename R>
    static constexpr auto evaluate(L l, R r) {
        return l * r;
    }
};
inline constexpr MultiplyInteger multiply_integer;

namespace Multiply {
template<typename L, typename R>
    requires(std::integral<L> && std::integral<R>)
constexpr auto specialization(op, type_array<L, R>) noexcept {
    return multiply_integer;
}
} // namespace Multiply

struct MultiplyFloat : operation_base<MultiplyFloat> {
    using parent = Multiply::op;

    // floating point multiplication is neither associative nor commutative

    template<typename L, typename R>
    static constexpr auto evaluate(L l, R r) {
        return l * r;
    }
};
inline constexpr MultiplyFloat multiply_float;

namespace Multiply {
template<typename L, typename R>
    requires(std::floating_point<L> && std::floating_point<R>)
constexpr auto specialization(op, type_array<L, R>) noexcept {
    return multiply_float;
}
} // namespace Multiply

} // namespace ac
