// Copyright 2025 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/meta/detail/const_volatile.hpp>

namespace ac {

/// @file Traits that add or remove type qualifiers:
/// - `const`;
/// - `volatile`;
/// - cv for `const` and `volatile`.
/// If the type is a reference, then the referenced type is modified.

template<class T>
using add_inner_const_t = typename add_inner_const<T>::type;
template<class T>
using remove_inner_const_t = typename remove_inner_const<T>::type;

template<class T>
using add_inner_volatile_t = typename add_inner_volatile<T>::type;
template<class T>
using remove_inner_volatile_t = typename remove_inner_volatile<T>::type;

template<class T>
using add_inner_cv_t = typename add_inner_cv<T>::type;
template<class T>
using remove_inner_cv_t = typename remove_inner_cv<T>::type;

} // namespace ac
