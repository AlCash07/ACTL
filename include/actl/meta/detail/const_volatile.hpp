// Copyright 2025 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <type_traits>

namespace ac {

template<class T>
struct add_inner_const {
    using type = T const;
};
template<class T>
struct add_inner_const<T&> {
    using type = T const&;
};
template<class T>
struct add_inner_const<T&&> {
    using type = T const&&;
};

template<class T>
struct remove_inner_const {
    using type = std::remove_const_t<T>;
};
template<class T>
struct remove_inner_const<T const&> {
    using type = T&;
};
template<class T>
struct remove_inner_const<T const&&> {
    using type = T&&;
};

template<class T>
struct add_inner_volatile {
    using type = T volatile;
};
template<class T>
struct add_inner_volatile<T&> {
    using type = T volatile&;
};
template<class T>
struct add_inner_volatile<T&&> {
    using type = T volatile&&;
};

template<class T>
struct remove_inner_volatile {
    using type = std::remove_volatile_t<T>;
};
template<class T>
struct remove_inner_volatile<T volatile&> {
    using type = T&;
};
template<class T>
struct remove_inner_volatile<T volatile&&> {
    using type = T&&;
};

template<class T>
struct add_inner_cv : add_inner_const<typename add_inner_volatile<T>::type> {};

template<class T>
struct remove_inner_cv
    : remove_inner_const<typename remove_inner_volatile<T>::type> {};

} // namespace ac
