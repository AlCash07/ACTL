// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/numeric/arithmetic/additive/negate_scalar.hpp>

namespace ac {

struct Negate : operation_base<Negate> {
    using operation_category = additive_operation;

    template<typename T>
        requires std::is_arithmetic_v<T>
    friend constexpr auto specialization(Negate, type_array<T>) noexcept {
        return negate_scalar;
    }
};
inline constexpr Negate negate;

// unary minus
template<typename T>
    requires EnableOperators<T>
constexpr auto operator-(T&& t) {
    return negate(std::forward<T>(t));
}

} // namespace ac
