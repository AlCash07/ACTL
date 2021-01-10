// Copyright 2021 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/category/utility/category_level.hpp>

namespace ac {

namespace detail {

template <class T>
struct category_wrap {
    using type = T;
};

}  // namespace detail

template <class... Ts>
using common_category_t = typename decltype((... && detail::category_wrap<Ts>{}))::type;

namespace detail {

template <class T>
constexpr auto base(category_wrap<T>) {
    return category_wrap<typename T::base>{};
}

template <class T, class U>
constexpr auto merge(category_wrap<T> lhs, category_wrap<U> rhs) {
    if constexpr (std::is_same_v<T, U>)
        return lhs;
    else
        return base(lhs) && base(rhs);
}

template <template <class...> class C, class... Ts, class... Us>
constexpr auto merge(category_wrap<C<Ts...>>, category_wrap<C<Us...>>) {
    return category_wrap<C<common_category_t<Ts, Us>...>>{};
}

template <class T, class U>
constexpr auto operator&&(category_wrap<T> lhs, category_wrap<U> rhs) {
    constexpr index llevel = category_level_v<T>;
    constexpr index rlevel = category_level_v<U>;
    if constexpr (llevel == rlevel)
        return merge(lhs, rhs);
    else if constexpr (llevel < rlevel)
        return lhs && base(rhs);
    else
        return base(lhs) && rhs;
}

}  // namespace detail

}  // namespace ac
