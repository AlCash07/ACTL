// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/category/utility/common_category.hpp>
#include <actl/category/utility/nesting_depth.hpp>

namespace ac {

namespace detail {

template <class Category, index Depth>
struct cdp {  // category-depth pair
    using category = Category;
    static constexpr index depth = Depth;
};

template <class T, index DepthT, class U, index DepthU>
constexpr auto operator||(cdp<T, DepthT> lhs, cdp<U, DepthU> rhs) {
    if constexpr (DepthT == DepthU)
        return cdp<common_category_t<T, U>, DepthT>{};
    else if constexpr (DepthT < DepthU)
        return rhs;
    else
        return lhs;
}

}  // namespace detail

template <class... Ts>
using major_category = decltype((... || detail::cdp<category_t<Ts>, nesting_depth_v<Ts>>{}));

template <class... Ts>
using major_category_t = typename major_category<Ts...>::category;

}  // namespace ac
