// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/core/none.hpp>
#include <actl/operation/overload/overload.hpp>
#include <algorithm>

namespace ac {

template<Operation Op, typename ArgsArray, typename Policy>
struct policy_overload {
    struct is_resolved;

    static constexpr Op resolve(Op&& op, Policy) {
        return std::move(op);
    }

    static constexpr Op const& resolve(Op const& op, Policy) {
        return op;
    }
};

template<Operation Op, typename ArgsArray, typename Policy>
struct overload_resolver : policy_overload<Op, ArgsArray, Policy> {};

template<typename... Args, typename Op>
constexpr decltype(auto) resolve_overload(Op&& op) {
    return overload_resolver<raw_t<Op>, type_array<raw_t<Args>...>, none>::
        resolve(std::forward<Op>(op), none{});
}

template<Operation Op, typename ArgsArray, typename Policy>
inline constexpr bool is_overload_resolved_v = requires {
    typename overload_resolver<raw_t<Op>, ArgsArray, Policy>::is_resolved;
};

template<typename Op, typename... Args, typename Policy>
    requires requires { overload<Op, Args...>::formula; }
struct overload_resolver<Op, type_array<Args...>, Policy> {
    template<typename Op1>
    static constexpr auto resolve(Op1&& op, Policy policy) {
        using Formula =
            std::remove_const_t<decltype(overload<Op, Args...>::formula)>;
        return overload_resolver<Formula, type_array<Args...>, Policy>::resolve(
            Formula{}, policy
        );
    }
};

} // namespace ac
