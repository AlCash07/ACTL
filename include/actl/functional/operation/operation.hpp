// Copyright 2019 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/functional/operation/expression.hpp>
#include <actl/functional/operation/expression_operation.hpp>
#include <actl/functional/operation/overload_resolution.hpp>
#include <actl/traits/nth_type.hpp>

namespace ac {

template <class Derived>
struct operation {
    using category = operation_tag;
    struct enable_operators;

    constexpr const Derived& derived() const {
        return static_cast<const Derived&>(*this);
    }

    template <class... Ts, class T = remove_cvref_t<nth_type_t<0, Ts...>>,
              enable_int_if<!is_out<T, true>::value && !is_policy_v<T>> = 0>
    constexpr decltype(auto) operator()(Ts&&... xs) const {
        decltype(auto) op = resolve<raw_t<Ts>...>();
        if constexpr ((... || is_out<remove_cvref_t<Ts>>::value)) {
            static_assert(1 == (... + is_out<remove_cvref_t<Ts>>::value),
                          "single inout argument expected");
            auto& dst = find_dst(xs...);
            op.evaluate_to(dst, remove_inout(xs)...);
            return dst;
        } else {
            return make_expression(std::forward<decltype(op)>(op), std::forward<Ts>(xs)...);
        }
    }

    template <class T, class... Ts>
    constexpr T operator()(out_t<false, T> dst, const Ts&... xs) const {
        resolve<raw_t<T>, raw_t<Ts>...>().evaluate_to(dst.x, xs...);
        return dst.x;
    }

    template <class Policy, enable_int_if<is_policy_v<remove_cvref_t<Policy>>> = 0>
    constexpr auto operator()(Policy&& policy) const {
        return tuned_operation<Derived, Policy>{derived(), std::forward<Policy>(policy)};
    }

    template <class... Ts>
    constexpr decltype(auto) resolve() const {
        return overload_helper<Ts...>::resolve(derived());
    }

    template <class... Ts>
    constexpr decltype(auto) resolve_nested() const {
        constexpr auto major_depth = nesting_depth_v<major_category_t<category_t<raw_t<Ts>>...>>;
        return overload_helper<detail::value_type_if_t<
            nesting_depth_v<category_t<raw_t<Ts>>> == major_depth, Ts>...>::resolve(derived());
    }
};

}  // namespace ac
