// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/types.hpp>
#include <type_traits>

namespace ac {

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
using value_type_t = typename detail::traits_t<T>::value_type;

template <class T>
using reference_t = typename detail::reference<T>::type;

template <class T>
using pointer_t = typename detail::pointer<T>::type;

template <class T>
using difference_t = typename detail::traits_t<T>::difference_type;

template <class T>
using iterator_t = typename detail::iterator<T>::type;

template <class T>
using size_type_t = typename detail::traits_t<T>::size_type;

}  // namespace ac
