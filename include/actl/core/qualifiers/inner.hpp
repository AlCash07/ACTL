// Copyright 2025 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <type_traits>

namespace ac {

/* const */

template<typename T>
struct add_inner_const {
    using type = T const;
};
template<typename T>
using add_inner_const_t = typename add_inner_const<T>::type;

template<typename T>
struct remove_inner_const {
    using type = std::remove_const_t<T>;
};
template<typename T>
using remove_inner_const_t = typename remove_inner_const<T>::type;

/* volatile */

template<typename T>
struct add_inner_volatile {
    using type = T volatile;
};
template<typename T>
using add_inner_volatile_t = typename add_inner_volatile<T>::type;

template<typename T>
struct remove_inner_volatile {
    using type = std::remove_volatile_t<T>;
};
template<typename T>
using remove_inner_volatile_t = typename remove_inner_volatile<T>::type;

/* cv - const volatile */

template<typename T>
struct add_inner_cv : add_inner_const<typename add_inner_volatile<T>::type> {};
template<typename T>
using add_inner_cv_t = typename add_inner_cv<T>::type;

template<typename T>
struct remove_inner_cv
    : remove_inner_const<typename remove_inner_volatile<T>::type> {};
template<typename T>
using remove_inner_cv_t = typename remove_inner_cv<T>::type;

/* implementation */

template<typename T>
struct add_inner_const<T&> {
    using type = T const&;
};
template<typename T>
struct add_inner_const<T&&> {
    using type = T const&&;
};

template<typename T>
struct remove_inner_const<T const&> {
    using type = T&;
};
template<typename T>
struct remove_inner_const<T const&&> {
    using type = T&&;
};

template<typename T>
struct add_inner_volatile<T&> {
    using type = T volatile&;
};
template<typename T>
struct add_inner_volatile<T&&> {
    using type = T volatile&&;
};

template<typename T>
struct remove_inner_volatile<T volatile&> {
    using type = T&;
};
template<typename T>
struct remove_inner_volatile<T volatile&&> {
    using type = T&&;
};

} // namespace ac
