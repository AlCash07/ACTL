// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/core/if_else.hpp>
#include <actl/lifetime/conversion/as.hpp>
#include <actl/operation/arg.hpp>

namespace ac {

struct allow_promotion {
    struct is_policy;
};

template<Operation Op, typename L, typename R>
    requires(std::is_arithmetic_v<L> && std::is_arithmetic_v<R>)
constexpr auto apply_policy(Op&& op, allow_promotion, type_array<L, R>) {
    using CT = std::common_type_t<L, R>;
    return std::forward<Op>(op)(as<CT>(l_), as<CT>(r_));
}

template<typename L, typename R>
    requires(std::is_arithmetic_v<L> && std::is_arithmetic_v<R>)
constexpr auto apply_policy(IfElse, allow_promotion, type_array<L, R>) {
    using CT = std::common_type_t<L, R>;
    return if_else(arg<0, 3>, as<CT>(arg<1, 3>), as<CT>(arg<2, 3>));
}

} // namespace ac
