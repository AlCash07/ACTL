// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/core/as.hpp>
#include <actl/core/if_else.hpp>
#include <actl/core/scalar_operation.hpp>
#include <actl/operation/operation/composite_operation.hpp>

namespace ac {

struct allow_promotion {
    struct is_policy;
};

struct Promotion {
    static constexpr size_t inner_count = 1;

    template<typename L, typename R>
    static constexpr auto evaluate(
        IfElse op, bool condition, L const& l, R const& r
    ) {
        using CT = std::common_type_t<L, R>;
        return IfElse::evaluate(condition, as<CT>(l), as<CT>(r));
    }

    template<Operation Op, typename... Ts>
    static constexpr auto evaluate(Op const& op, Ts const&... xs) {
        using CT = std::common_type_t<decltype(xs)...>;
        return op.evaluate(eval(as<CT>(xs))...);
    }
};

constexpr auto apply_policy(ScalarOperation auto const& op, allow_promotion) {
    return operation_composer<Promotion>{}(op);
}

} // namespace ac
