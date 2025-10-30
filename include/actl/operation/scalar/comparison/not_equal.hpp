// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/operation/scalar/comparison/equal.hpp>
#include <actl/operation/scalar/logical/logical_not.hpp>

namespace ac {

struct not_equal_f : operation<not_equal_f> {
    using operation_category = equality_operation_tag;

    static constexpr bool is_commutative = true;

    static constexpr auto formula = !equal;
};
inline constexpr not_equal_f not_equal;

template<typename L, typename R>
    requires EnableOperators<L, R>
constexpr auto operator!=(L&& l, R&& r) {
    return not_equal(pass<L>(l), pass<R>(r));
}

} // namespace ac
