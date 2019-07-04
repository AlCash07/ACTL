/***************************************************************************************************
 * Extension of the standard type traits.
 ***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/types.hpp>
#include <type_traits>

namespace ac {

template <bool B, class T>
struct add_const_if : std::conditional<B, const T, T> {};

template <bool B, class T>
using add_const_if_t = typename add_const_if<B, T>::type;

template <index N>
using index_constant = std::integral_constant<index, N>;

// TODO: use std::remove_cvref_t when C++20 is out.
template <class T>
using remove_cvref_t = std::remove_cv_t<std::remove_reference_t<T>>;

template <bool B>
using enable_int_if = std::enable_if_t<B, int>;

template <size_t N, class... Ts>
struct nth_type;

template <size_t N, class... Ts>
using nth_t = typename nth_type<N, Ts...>::type;

template <class T, class... Ts>
struct nth_type<0, T, Ts...> {
    using type = T;
};

template <size_t N, class T, class... Ts>
struct nth_type<N, T, Ts...> {
    using type = nth_t<N - 1, Ts...>;
};

/* Dependent types */

namespace detail {

template <class T, class = void>
struct traits {
    using type = T;
};

template <class T>
struct traits<T*, std::enable_if_t<std::is_object_v<T>>> {
    struct type {
        using value_type = std::remove_cv_t<T>;
        using reference = T&;
        using pointer = T*;
        using difference_type = index;
    };
};

template <class T>
struct traits<T* const> : traits<T*> {};

template <class T, index N>
struct traits<T[N]> {
    struct type : traits<T*>::type {
        using value_type = T;
        using iterator = T*;
        using size_type = index;
    };
};

template <class T>
using traits_t = typename traits<T>::type;

template <class T, class = void>
struct reference {
    using type = typename traits_t<T>::reference;
};

template <class T>
struct reference<const T, std::void_t<typename traits_t<T>::const_reference>> {
    using type = typename traits_t<T>::const_reference;
};

template <class T, class = void>
struct pointer {
    using type = typename traits_t<T>::pointer;
};

template <class T>
struct pointer<const T, std::void_t<typename traits_t<T>::const_pointer>> {
    using type = typename traits_t<T>::const_pointer;
};

template <class T, class = void>
struct iterator {
    using type = typename traits_t<T>::iterator;
};

template <class T>
struct iterator<const T, std::void_t<typename traits_t<T>::const_iterator>> {
    using type = typename traits_t<T>::const_iterator;
};

}  // namespace detail

template <class T>
using value_t = typename detail::traits_t<T>::value_type;

template <class T>
using reference_t = typename detail::reference<T>::type;

template <class T>
using pointer_t = typename detail::pointer<T>::type;

template <class T>
using iterator_t = typename detail::iterator<T>::type;

template <class T>
using difference_t = typename detail::traits_t<T>::difference_type;

template <class T>
using size_type_t = typename detail::traits_t<T>::size_type;

}  // namespace ac
