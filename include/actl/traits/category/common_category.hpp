// Copyright 2021 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/traits/category/utility/category_level.hpp>

namespace ac {

template <class T, class U>
constexpr auto common_category(T, U) {
    constexpr index levelT = category_level_v<T>;
    constexpr index levelU = category_level_v<U>;
    if constexpr (levelT == levelU) {
        if constexpr (std::is_same_v<T, U>)
            return T{};
        else
            return common_category(typename T::base{}, typename U::base{});
    } else if constexpr (levelT < levelU)
        return common_category(T{}, typename U::base{});
    else
        return common_category(typename T::base{}, U{});
}

template <class... Ts>
using common_category_t = decltype(common_category(std::declval<Ts>()...));

}  // namespace ac
