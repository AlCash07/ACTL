// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/numeric/logic/logical_operation.hpp>

namespace ac {

namespace LogicalOr {
struct op : operation_base<op> {
    using operation_category = logical_operation;

    static constexpr auto identity_element = constant<false>{};
};
} // namespace LogicalOr
inline constexpr LogicalOr::op logical_or;

template<typename L, typename R>
    requires EnableOperators<L, R>
constexpr auto operator||(L&& l, R&& r) {
    return logical_or(std::forward<L>(l), std::forward<R>(r));
}

} // namespace ac
