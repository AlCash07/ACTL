// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/traits/category/common_category.hpp>
#include <actl/traits/dependent.hpp>

namespace ac::detail {

template <class Tag, index Depth>
struct cdp {  // category-depth pair
    using type = Tag;
    static constexpr index value = Depth;
};

template <class T, index DT, class U, index DU>
constexpr auto operator||(cdp<T, DT> lhs, cdp<U, DU> rhs) {
    if constexpr (DT == DU)
        return cdp<common_category_t<T, U>, DT>{};
    else if constexpr (DT < DU)
        return rhs;
    else
        return lhs;
}

template <class T, class = void>
struct type_depth : index_constant<0> {};

template <class T>
constexpr index type_depth_v = type_depth<T>::value;

template <class T>
struct type_depth<T, std::void_t<typename category_t<T>::has_nested>>
    : index_constant<type_depth_v<value_type_t<T>> + 1> {};

template <class... Ts>
using major_cdp =
    remove_cvref_t<decltype((... || std::declval<cdp<category_t<Ts>, type_depth_v<Ts>>>()))>;

template <class... Ts>
using major_category_t = typename major_cdp<Ts...>::type;

}  // namespace ac::detail
