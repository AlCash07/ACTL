/***************************************************************************************************
 * Copyright 2020 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/util/type_traits.hpp>

namespace ac::math {

template <class T>
struct raw {
    using type = T;
};

template <class T>
using raw_t = typename raw<remove_cvref_t<T>>::type;

struct scalar_tag {};

template <class T, class = void>
struct category_impl {
    using type = scalar_tag;
};

template <class T>
struct category_impl<T, std::void_t<typename T::category>> {
    using type = typename T::category;
};

template <class T>
struct category : category_impl<T> {};

template <class T>
using category_t = typename category<raw_t<T>>::type;

namespace detail {

template <class Tag, class = void>
struct category_depth : index_constant<0> {};

template <class T>
inline constexpr index category_depth_v = category_depth<T>::value;

template <class Tag>
struct category_depth<Tag, std::void_t<typename Tag::base>>
    : index_constant<1 + category_depth_v<typename Tag::base>> {};

template <class T, class U>
inline constexpr auto common_category(T, U) {
    constexpr index DT = category_depth_v<T>;
    constexpr index DU = category_depth_v<U>;
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

template <class Tag, int Depth>
struct cdp {  // category-depth pair
    using type = Tag;
    static constexpr int value = Depth;
};

template <class T, int DT, class U, int DU>
inline constexpr auto operator || (cdp<T, DT> lhs, cdp<U, DU> rhs) {
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
inline constexpr index type_depth_v = type_depth<raw_t<T>>::value;

template <class T>
struct type_depth<T, std::void_t<typename category_t<T>::has_nested>>
    : index_constant<type_depth_v<value_t<T>> + 1> {};

template <class... Ts>
using major_cdp =
    remove_cvref_t<decltype((... || std::declval<cdp<category_t<Ts>, type_depth_v<Ts>>>()))>;

template <class... Ts>
using major_category = typename major_cdp<Ts...>::type;

template <bool B, class T>
struct value_if {
    using type = value_t<raw_t<T>>;
};

template <class T>
struct value_if<false, T> {
    using type = T;
};

template <bool B, class T>
using value_if_t = typename value_if<B, T>::type;

}  // namespace detail

// pass is the same as std::forward except it converts reference into const reference
template <class T>
inline constexpr T&& pass(std::remove_reference_t<T>& x) {
    return static_cast<T&&>(x);
}

template <class T>
inline constexpr const T&& pass(std::remove_reference_t<T>&& x) {
    static_assert(!std::is_lvalue_reference_v<T>, "can not pass an rvalue as an lvalue");
    return static_cast<const T&&>(x);
}

template <class T>
inline constexpr T eval(const T& x) {
    return x;
}

template <class T, size_t N>
inline auto& eval(const T (&x)[N]) {
    return x;
}

}  // namespace ac::math
