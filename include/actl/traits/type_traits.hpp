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

}  // namespace ac
