// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/category/category.hpp>
#include <actl/category/utility/major_category.hpp>

namespace ac {

template <class Op, class = void>
struct default_overload {
    struct is_unchanged;

    static constexpr const Op& resolve(const Op& op) {
        return op;
    }

    static constexpr Op resolve(Op&& op) {
        return std::move(op);
    }
};

template <class Op>
struct default_overload<Op, std::void_t<decltype(Op::formula)>> {
    static constexpr auto formula = Op::formula;
};

template <class Op, class Category, class... Ts>
struct overload : overload<Op, typename Category::base, Ts...> {};

template <class Op, class... Ts>
struct overload<Op, unclassified_tag, Ts...> : default_overload<Op> {};

template <class Op, class... Ts>
using major_overload = overload<Op, major_category_t<Ts...>, Ts...>;

} // namespace ac
