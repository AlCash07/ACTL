// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/functional/operation/argument_traits.hpp>

namespace ac::math::detail {

template <class T, class U>
constexpr auto common_category(T, U) {
    constexpr index DT = category_level_v<T>;
    constexpr index DU = category_level_v<U>;
    if constexpr (DT == DU) {
        if constexpr (std::is_same_v<T, U>) {
            return T{};
        } else {
            static_assert(DT > 0, "incompatible categories");
            return common_category(typename T::base{}, typename U::base{});
        }
    } else if constexpr (DT < DU) {
        return common_category(T{}, typename U::base{});
    } else {
        return common_category(typename T::base{}, U{});
    }
}

template <class Tag, index Depth>
struct cdp {  // category-depth pair
    using type = Tag;
    static constexpr index value = Depth;
};

template <class T, index DT, class U, index DU>
constexpr auto operator || (cdp<T, DT> lhs, cdp<U, DU> rhs) {
    if constexpr (DT == DU) {
        return cdp<decltype(common_category(T{}, U{})), DT>{};
    } else if constexpr (DT < DU) {
        return rhs;
    } else {
        return lhs;
    }
}

template <class T, class = void>
struct type_depth : index_constant<0> {};

template <class T>
constexpr index type_depth_v = type_depth<raw_t<T>>::value;

template <class T>
struct type_depth<T, std::void_t<typename category_t<T>::has_nested>>
    : index_constant<type_depth_v<value_t<T>> + 1> {};

template <class... Ts>
using major_cdp =
    remove_cvref_t<decltype((... || std::declval<cdp<category_t<Ts>, type_depth_v<Ts>>>()))>;

template <class... Ts>
using major_category_t = typename major_cdp<Ts...>::type;

}  // namespace ac::math::detail
