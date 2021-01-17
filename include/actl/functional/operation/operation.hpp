// Copyright 2019 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/functional/operation/expression.hpp>
#include <actl/functional/operation/expression_operation.hpp>
#include <actl/functional/operation/inout.hpp>
#include <actl/functional/operation/overload.hpp>
#include <actl/functional/operation/tuned_operation.hpp>

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

    template <class... Ts, enable_int_if<(... || is_inout_v<Ts>)> = 0>
    constexpr decltype(auto) operator()(Ts&&... xs) const& {
        static_assert(1 == (... + is_inout_v<Ts>), "single inout argument expected");
        decltype(auto) op = derived().template resolve<Ts...>();
        auto& dst = find_dst(xs...);
        op.evaluate_to(dst, remove_inout(xs)...);
        return dst;
    }

    template <class... Ts, enable_int_if<!(... || is_inout_v<Ts>)> = 0>
    constexpr decltype(auto) operator()(Ts&&... xs) const& {
        return make_expression(derived(), std::forward<Ts>(xs)...);
    }

    template <class... Ts, enable_int_if<!(... || is_inout_v<Ts>)> = 0>
    constexpr decltype(auto) operator()(Ts&&... xs) && {
        return make_expression(static_cast<Derived&&>(*this), std::forward<Ts>(xs)...);
    }

    template <class... Ts>
    constexpr decltype(auto) resolve() const {
        using major_t = major_category_t<category_t<raw_t<Ts>>...>;
        return overload<Derived, major_t, raw_t<Ts>...>::resolve(derived());
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
