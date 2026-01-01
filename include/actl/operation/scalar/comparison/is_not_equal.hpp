// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/numeric/logic/logical_not.hpp>
#include <actl/operation/scalar/comparison/is_equal.hpp>

namespace ac {

struct NotEqual : operation_base<NotEqual> {
    using operation_category = equality_operation_tag;

    static constexpr bool is_commutative = true;

    static constexpr auto formula = !is_equal;
};
inline constexpr NotEqual is_not_equal;

template<typename L, typename R>
    requires EnableOperators<L, R>
constexpr auto operator!=(L&& l, R&& r) {
    return is_not_equal(pass<L>(l), pass<R>(r));
}

} // namespace ac
