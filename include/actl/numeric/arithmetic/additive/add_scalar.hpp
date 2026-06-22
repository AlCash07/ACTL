// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/numeric/arithmetic/arithmetic_operation.hpp>

namespace ac {

struct Add;

struct AddInteger : operation_base<AddInteger> {
    using parent = Add;

    static constexpr bool is_associative = true;
    static constexpr bool is_commutative = true;

    template<typename L, typename R>
    static constexpr auto evaluate(L l, R r) {
        return l + r;
    }
};
inline constexpr AddInteger add_integer;

struct AddFloat : operation_base<AddFloat> {
    using parent = Add;

    // floating point addition is neither associative nor commutative

    template<typename L, typename R>
    static constexpr auto evaluate(L l, R r) {
        return l + r;
    }
};
inline constexpr AddFloat add_float;

} // namespace ac
