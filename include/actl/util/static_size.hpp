/***************************************************************************************************
 * Copyright 2019 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <type_traits>

namespace ac {

inline constexpr index dynamic_size = -1;

template <class T>
struct static_size : index_constant<dynamic_size> {};

template <class T, index N>
struct static_size<T[N]> : index_constant<N> {};

template <class T>
inline constexpr index static_size_v = static_size<std::remove_const_t<T>>::value;

}  // namespace ac
