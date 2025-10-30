// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/operation/scalar/scalar_operation.hpp>

namespace ac {

namespace scalar {

struct less_f : scalar_operation<less_f, 2> {
    using operation_category = ordering_operation_tag;

    template<typename L, typename R>
    static constexpr bool eval_scalar(L l, R r) {
        return l < r;
    }
};
inline constexpr less_f less;

} // namespace scalar

struct less_f : operation<less_f> {
    using operation_category = ordering_operation_tag;

    static constexpr auto formula = scalar::less;
};
inline constexpr less_f less;

template<typename L, typename R>
    requires EnableOperators<L, R>
constexpr auto operator<(L&& l, R&& r) {
    return less(pass<L>(l), pass<R>(r));
}

} // namespace ac
