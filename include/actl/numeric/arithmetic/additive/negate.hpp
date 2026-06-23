// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/numeric/arithmetic/arithmetic_operation.hpp>

namespace ac {

namespace Negate {
struct op : operation_base<op> {
    using operation_category = additive_operation;
};
} // namespace Negate
inline constexpr Negate::op negate;

// unary minus
template<typename T>
    requires EnableOperators<T>
constexpr auto operator-(T&& t) {
    return negate(std::forward<T>(t));
}

} // namespace ac
