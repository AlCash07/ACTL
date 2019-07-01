/***************************************************************************************************
 * Copyright 2019 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/types.hpp>
#include <actl/util/type_traits.hpp>

namespace ac {

template <class T, class C>
struct is_string : std::false_type {};

template <class C, index N>
struct is_string<C[N], C> : std::true_type {};

template <class T, class Char = char>
inline constexpr bool is_string_v = is_string<remove_cvref_t<T>, Char>::value;

}  // namespace ac
