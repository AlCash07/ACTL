// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/numeric/bit/bit_operation.hpp>

namespace ac {

namespace BitAnd {
struct op : operation_base<op> {
    using operation_category = bitwise_operation;

    static constexpr auto identity_element = constant<-1>{};
};
} // namespace BitAnd
inline constexpr BitAnd::op bit_and;

template<typename L, typename R>
    requires EnableOperators<L, R>
constexpr auto operator&(L&& l, R&& r) {
    return bit_and(std::forward<L>(l), std::forward<R>(r));
}

template<typename L, typename R>
    requires EnableOperators<L, R>
constexpr decltype(auto) operator&=(L&& l, R&& r) {
    return bit_and(inout{std::forward<L>(l)}, std::forward<R>(r));
}

} // namespace ac
