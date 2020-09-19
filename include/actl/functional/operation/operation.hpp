/***************************************************************************************************
 * Copyright 2019 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/functional/operation/expression.hpp>
#include <actl/functional/operation/expression_operation.hpp>
#include <actl/functional/operation/overload_resolution.hpp>

namespace ac::math {

template <class Derived>
struct operation {
    using category = operation_tag;

    constexpr const Derived& derived() const { return static_cast<const Derived&>(*this); }

    template <class... Ts, class T = remove_cvref_t<nth_t<0, Ts...>>,
              enable_int_if<!is_out<T, true>::value && !is_policy_v<T>> = 0>
    constexpr decltype(auto) operator()(Ts&&... xs) const {
        decltype(auto) op = resolve<Ts...>();
        if constexpr ((... || is_out<remove_cvref_t<Ts>>::value)) {
            static_assert(1 == (... + is_out<remove_cvref_t<Ts>>::value),
                          "single inplace argument expected");
            auto& dst = find_dst(xs...);
            op.evaluate_to(dst, remove_inplace(xs)...);
            return dst;
        } else {
            return make_expression(std::forward<decltype(op)>(op), std::forward<Ts>(xs)...);
        }
    }

    template <class T, class... Ts>
    constexpr T operator()(out<false, T> dst, const Ts&... xs) const {
        resolve<T, Ts...>().evaluate_to(dst.x, xs...);
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
        return overload_helper<
            detail::value_if_t<detail::type_depth_v<Ts> == detail::major_cdp<Ts...>::value,
                               Ts>...>::resolve(derived());
    }
};

}  // namespace ac::math
