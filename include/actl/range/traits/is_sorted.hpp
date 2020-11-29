/***************************************************************************************************
 * Copyright 2020 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/range/traits/range_traits.hpp>

namespace ac {

template <class T, class = void>
struct is_sorted_range : std::false_type {};

template <class T>
struct is_sorted_range<T, std::void_t<typename range_traits<T>::is_sorted>> : std::true_type {};

template <class T>
constexpr bool is_sorted_range_v = is_sorted_range<T>::value;

}  // namespace ac
