/***************************************************************************************************
 * Copyright 2019 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/types.hpp>
#include <string>
#include <type_traits>

namespace ac {

template <class T>
struct is_string : std::false_type {};

template <class C, class T, class A>
struct is_string<std::basic_string<C, T, A>> : std::true_type {};

template <int N>
struct is_string<char[N]> : std::true_type {};

template <>
struct is_string<zstring> : std::true_type {};

template <>
struct is_string<czstring> : std::true_type {};

template <class T>
inline constexpr bool is_string_v = is_string<T>::value;

}  // namespace ac
