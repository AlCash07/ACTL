// Copyright 2019 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/operation/inout.hpp>
#include <actl/operation/overload/resolve_expression.hpp>
#include <actl/operation/overload/resolve_overload.hpp>
#include <actl/operation/policy/resolve_overload.hpp>

namespace ac {

template <class Derived>
struct operation
{
    using category = operation_tag;
    struct enable_operators;

    static constexpr bool is_associative = false;
    static constexpr bool is_commutative = false;

    constexpr const Derived& derived() const noexcept
    {
        return static_cast<const Derived&>(*this);
    }

    template <class... Ts, enable_int_if<is_any_inout_v<Ts...>> = 0>
    constexpr decltype(auto) operator()(Ts&&... xs) const&
    {
        static_assert(
            1 == (... + is_inout_v<Ts>), "single inout argument expected");
        auto&& op =
            resolve_overload_if_can<Ts...>(default_context{}, derived());
        auto& dst = find_dst(xs...);
        op.evaluate_to(dst, remove_inout(xs)...);
        return dst;
    }

    template <class... Ts, enable_int_if<!is_any_inout_v<Ts...>> = 0>
    constexpr auto operator()(Ts&&... xs) const&
    {
        return expression{derived(), std::forward<Ts>(xs)...};
    }

    template <class... Ts, enable_int_if<!is_any_inout_v<Ts...>> = 0>
    constexpr auto operator()(Ts&&... xs) &&
    {
        return expression{
            static_cast<Derived&&>(*this), std::forward<Ts>(xs)...};
    }
};

} // namespace ac
