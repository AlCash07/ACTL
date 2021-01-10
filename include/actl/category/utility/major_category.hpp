// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/category/utility/common_category.hpp>
#include <actl/category/utility/nesting_depth.hpp>

namespace ac {

namespace detail {

template <class T, class U>
constexpr auto operator||(category_wrap<T> lhs, category_wrap<U> rhs) {
    constexpr auto ldepth = nesting_depth_v<T>;
    constexpr auto rdepth = nesting_depth_v<U>;
    if constexpr (ldepth == rdepth)
        return lhs && rhs;
    else if constexpr (ldepth < rdepth)
        return rhs;
    else
        return lhs;
}

}  // namespace detail

template <class... Ts>
using major_category_t = typename decltype((... || detail::category_wrap<Ts>{}))::type;

}  // namespace ac
