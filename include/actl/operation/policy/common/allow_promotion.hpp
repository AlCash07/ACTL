// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/operation/core/composite_operation.hpp>
#include <actl/operation/scalar/arithmetic/add.hpp>
#include <actl/operation/scalar/common/cast.hpp>
#include <actl/operation/scalar/common/select.hpp>

namespace ac {

struct allow_promotion {
    struct is_policy;
};

struct Promotion {
    static constexpr index inner_count = 1;

    template <class T, class U>
    static constexpr auto evaluate(
        select_f op, bool condition, const T& lhs, const U& rhs) {
        using CT = std::common_type_t<T, U>;
        return op.evaluate(condition, cast<CT>(lhs), cast<CT>(rhs));
    }

    template <class Op, class... Ts>
    static constexpr auto evaluate(const Op& op, const Ts&... xs) {
        using CT = std::common_type_t<Ts...>;
        return op.evaluate(cast<CT>(xs)...);
    }
};

template <
    class Op,
    enable_int_if<
        is_scalar_operation_v<Op> &&
        is_subcategory_of_v<typename Op::argument_category, scalar_tag>> = 0>
constexpr auto apply_policy(const Op& op, allow_promotion) {
    return operation_composer<Promotion>{}(op);
}

} // namespace ac
