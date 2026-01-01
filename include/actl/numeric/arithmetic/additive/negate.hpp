// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/numeric/arithmetic/arithmetic_operation.hpp>

namespace ac {

struct Negate : operation_base<Negate> {
    using operation_category = additive_operation;
};
inline constexpr Negate negate;

// unary minus
template<typename T>
    requires EnableOperators<T>
constexpr auto operator-(T&& x) {
    return negate(pass<T>(x));
}

struct NegateScalar : operation_base<NegateScalar> {
    using parent = Negate;

    template<typename T>
    static constexpr bool requirement = std::is_arithmetic_v<T>;

    template<typename T>
    static constexpr auto evaluate(T x) {
        return -x;
    }
};
AC_REGISTER_OVERLOAD(NegateScalar)
inline constexpr NegateScalar negate_scalar;

} // namespace ac
