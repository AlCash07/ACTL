// Copyright 2019 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/functional/operation/expression.hpp>
#include <actl/functional/operation/expression_operation.hpp>
#include <actl/functional/operation/inout.hpp>
#include <actl/functional/operation/overload_resolution.hpp>

namespace ac {

template <class Derived>
struct operation {
    using category = operation_tag;
    struct enable_operators;

    static constexpr bool is_associative = false;
    static constexpr bool is_commutative = false;

    constexpr const Derived& derived() const noexcept {
        return static_cast<const Derived&>(*this);
    }

    template <class... Ts>
    constexpr decltype(auto) operator()(Ts&&... xs) const {
        decltype(auto) op = derived().template resolve<raw_t<Ts>...>();
        if constexpr ((... || is_inout_v<Ts>)) {
            static_assert(1 == (... + is_inout_v<Ts>), "single inout argument expected");
            auto& dst = find_dst(xs...);
            op.evaluate_to(dst, remove_inout(xs)...);
            return dst;
        } else {
            return make_expression(std::forward<decltype(op)>(op), std::forward<Ts>(xs)...);
        }
    }

    template <class... Ts>
    constexpr decltype(auto) resolve() const {
        return overload_helper<remove_cvref_t<Ts>...>::resolve(derived());
    }

    template <class... Ts>
    constexpr decltype(auto) resolve_nested() const {
        constexpr auto major_depth = nesting_depth_v<major_category_t<category_t<raw_t<Ts>>...>>;
        return derived()
            .template resolve<detail::value_type_if_t<
                nesting_depth_v<category_t<raw_t<Ts>>> == major_depth, Ts>...>();
    }
};

}  // namespace ac
