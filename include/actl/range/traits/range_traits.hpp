/***************************************************************************************************
 * Copyright 2020 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <type_traits>

namespace ac {

template <class T>
struct range_traits {};

template <class T>
struct range_traits<const T> : range_traits<T> {};

template <class T, size_t N>
struct range_traits<T[N]> {
    struct is_container;
};

}  // namespace ac
