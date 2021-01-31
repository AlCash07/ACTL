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

namespace detail {

template <class T, class = void>
struct has_formula : std::false_type {};

template <class T>
struct has_formula<T, std::void_t<decltype(T::formula)>> : std::true_type {};

} // namespace detail

template <class... Ts>
struct resolve_t {
    template <class Op>
    constexpr decltype(auto) operator()(const Op& op) const {
        using Overload = major_overload<Op, Ts...>;
        if constexpr (detail::has_formula<Overload>::value)
            return std::remove_const_t<decltype(Overload::formula)>{};
        else
            return Overload::resolve(op);
    }

    template <class Op>
    constexpr decltype(auto) nested(const Op& op) const {
        constexpr auto major_depth = major_category<Ts...>::depth;
        return resolve_t<detail::value_type_if_t<
            nesting_depth_v<Ts> == major_depth,
            Ts>...>{}(op);
    }
};
template <class... Ts>
inline constexpr resolve_t<raw_t<Ts>...> resolve;

template <class Void, class... Ts>
struct is_overload_unchanged : std::false_type {};

template <class Op, class... Ts>
struct is_overload_unchanged<
    std::void_t<typename major_overload<Op, Ts...>::is_unchanged>,
    Op,
    Ts...> : std::true_type {};

template <class... Ts>
inline constexpr bool is_overload_unchanged_v =
    is_overload_unchanged<void, raw_t<Ts>...>::value;

} // namespace ac
