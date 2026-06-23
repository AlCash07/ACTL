// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/numeric/arithmetic/arithmetic_operation.hpp>

namespace ac {

namespace Subtract {
struct op : operation_base<op> {
    using operation_category = additive_operation;
};
} // namespace Subtract
inline constexpr Subtract::op subtract;

template<typename L, typename R>
    requires EnableOperators<L, R>
constexpr auto operator-(L&& l, R&& r) {
    return subtract(std::forward<L>(l), std::forward<R>(r));
}

template<typename L, typename R>
    requires EnableOperators<L, R>
constexpr decltype(auto) operator-=(L&& l, R&& r) {
    return subtract(inout{std::forward<L>(l)}, std::forward<R>(r));
}

} // namespace ac
