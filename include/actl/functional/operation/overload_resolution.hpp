// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/category/utility/major_category.hpp>
#include <actl/functional/operation/tuned_operation.hpp>

namespace ac {

template <class Op, class = void>
struct default_overload {
    static constexpr const Op& resolve(const Op& op) {
        return op;
    }
};

template <class Op>
struct default_overload<Op, std::void_t<decltype(Op::formula)>> {
    static constexpr auto resolve(Op) {
        return Op::formula;
    }
};

template <class Op, class Category, class... Ts>
struct overload : overload<Op, typename Category::base, Ts...> {};

template <class Op, class... Ts>
struct overload<Op, unclassified_tag, Ts...> : default_overload<Op> {};

// TODO: figure out a way to overload on the operation type and pass variadic template pack without
// this helper structure.
template <class... Ts>
struct overload_helper {
    template <class Op>
    static constexpr decltype(auto) resolve(const Op& op) {
        return overload<Op, major_category_t<category_t<raw_t<Ts>>...>, raw_t<Ts>...>::resolve(op);
    }

    template <class Op, class Policy>
    static constexpr decltype(auto) resolve(const tuned_operation<Op, Policy>& op) {
        return apply_policy_if_can(resolve(std::get<0>(op.t)), std::get<1>(op.t));
    }
};

}  // namespace ac
