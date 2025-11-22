// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/operation/scalar/scalar_operation.hpp>

namespace ac {

namespace scalar {

struct Less : operation_base<Less> {
    using operation_category = ordering_operation_tag;

    template<typename L, typename R>
    static constexpr bool evaluate(L l, R r) {
        return l < r;
    }
};
inline constexpr Less less;

} // namespace scalar

struct Less : operation_base<Less> {
    using operation_category = ordering_operation_tag;

    static constexpr auto formula = scalar::less;
};
inline constexpr Less less;

template<typename L, typename R>
    requires EnableOperators<L, R>
constexpr auto operator<(L&& l, R&& r) {
    return less(pass<L>(l), pass<R>(r));
}

} // namespace ac
