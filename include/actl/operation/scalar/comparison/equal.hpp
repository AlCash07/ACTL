// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/operation/scalar/scalar_operation.hpp>

namespace ac {

namespace scalar {

struct equal_f : scalar_operation<equal_f, 2> {
    using operation_category = equality_operation_tag;

    template<typename L, typename R>
    static constexpr bool eval_scalar(L l, R r) {
        return l == r;
    }
};
inline constexpr equal_f equal;

} // namespace scalar

struct equal_f : operation<equal_f> {
    using operation_category = equality_operation_tag;

    static constexpr bool is_commutative = true;

    static constexpr auto formula = scalar::equal;
};
inline constexpr equal_f equal;

template<typename L, typename R>
    requires EnableOperators<L, R>
constexpr auto operator==(L&& l, R&& r) {
    return equal(pass<L>(l), pass<R>(r));
}

} // namespace ac
