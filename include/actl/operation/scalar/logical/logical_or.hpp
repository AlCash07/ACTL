// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/operation/scalar/scalar_operation.hpp>

namespace ac {

namespace scalar {

struct logical_or_f : scalar_operation<logical_or_f, 2> {
    using operation_category = logical_operation_tag;

    static constexpr bool eval_scalar(bool l, bool r) {
        return l || r;
    }
};
inline constexpr logical_or_f logical_or;

} // namespace scalar

struct logical_or_f : operation_base<logical_or_f> {
    using operation_category = logical_operation_tag;

    static constexpr bool is_associative = true;
    static constexpr bool is_commutative = true;

    static constexpr auto formula = scalar::logical_or;
};
inline constexpr logical_or_f logical_or;

template<typename T>
struct identity_element<logical_or_f, T> {
    static constexpr T value() {
        return T{false};
    }
};

template<typename L, typename R>
    requires EnableOperators<L, R>
constexpr auto operator||(L&& l, R&& r) {
    return logical_or(pass<L>(l), pass<R>(r));
}

} // namespace ac
