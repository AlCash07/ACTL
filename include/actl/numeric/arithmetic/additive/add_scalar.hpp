// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/numeric/arithmetic/additive/add.hpp>

namespace ac {

struct AddInteger : operation_base<AddInteger> {
    using parent = Add::op;

    static constexpr bool is_associative = true;
    static constexpr bool is_commutative = true;

    template<typename L, typename R>
    static constexpr auto evaluate(L l, R r) {
        return l + r;
    }
};
inline constexpr AddInteger add_integer;

namespace Add {
template<typename L, typename R>
    requires(std::integral<L> && std::integral<R>)
constexpr auto specialization(op, type_array<L, R>) noexcept {
    return add_integer;
}
} // namespace Add

struct AddFloat : operation_base<AddFloat> {
    using parent = Add::op;

    // floating point addition is neither associative nor commutative

    template<typename L, typename R>
    static constexpr auto evaluate(L l, R r) {
        return l + r;
    }
};
inline constexpr AddFloat add_float;

namespace Add {
template<typename L, typename R>
    requires(std::floating_point<L> && std::floating_point<R>)
constexpr auto specialization(op, type_array<L, R>) noexcept {
    return add_float;
}
} // namespace Add

} // namespace ac
