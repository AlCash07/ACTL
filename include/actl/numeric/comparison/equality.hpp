// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/numeric/comparison/comparison_operation.hpp>
#include <actl/numeric/logic/logical_not.hpp>

namespace ac {

namespace IsEqual {
struct op : operation_base<op> {
    using operation_category = equality_operation;
};
} // namespace IsEqual
inline constexpr IsEqual::op is_equal;

template<typename L, typename R>
    requires EnableOperators<L, R>
constexpr auto operator==(L&& l, R&& r) {
    return is_equal(std::forward<L>(l), std::forward<R>(r));
}

inline constexpr auto is_not_equal = !is_equal;

template<typename L, typename R>
    requires EnableOperators<L, R>
constexpr auto operator!=(L&& l, R&& r) {
    return is_not_equal(std::forward<L>(l), std::forward<R>(r));
}

} // namespace ac
