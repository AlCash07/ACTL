// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/operation/scalar/scalar_operation.hpp>

namespace ac {

namespace scalar {

struct Neg : operation_base<Neg> {
    using operation_category = additive_operation_tag;

    template<typename T>
    static constexpr auto evaluate(T x) {
        return -x;
    }
};
inline constexpr Neg neg;

} // namespace scalar

struct Neg : operation_base<Neg> {
    using operation_category = additive_operation_tag;

    static constexpr auto formula = scalar::neg;
};
inline constexpr Neg neg;

template<typename T>
    requires EnableOperators<T>
constexpr auto operator-(T&& x) {
    return neg(pass<T>(x));
}

} // namespace ac
