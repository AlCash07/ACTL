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
struct default_overload {};

template <class Op>
struct default_overload<Op, std::void_t<decltype(Op::formula)>> {
    static constexpr auto formula = Op::formula;
};

template <class Op, class Category, class... Ts>
struct overload : overload<Op, typename Category::base, Ts...> {};

template <class Op, class... Ts>
struct overload<Op, unclassified_tag, Ts...> : default_overload<Op> {};

template <class Op, class... Ts>
struct primary_overload
    : overload<raw_t<Op>, major_category_t<raw_t<Ts>...>, raw_t<Ts>...> {
    template <
        class O =
            overload<raw_t<Op>, major_category_t<raw_t<Ts>...>, raw_t<Ts>...>>
    static constexpr auto resolve(Op&&)
        -> std::remove_const_t<decltype(O::formula)> {
        return std::remove_const_t<decltype(O::formula)>{};
    }
};

namespace detail {

template <class T, class Op, class = void>
struct has_resolve : std::false_type {};

template <class T, class Op>
struct has_resolve<T, Op, std::void_t<decltype(T::resolve(std::declval<Op>()))>>
    : std::true_type {};

} // namespace detail

template <class Op, class... Ts>
inline constexpr bool is_primary_overload_resolved_v =
    !detail::has_resolve<primary_overload<Op, Ts...>, Op>::value;

} // namespace ac
