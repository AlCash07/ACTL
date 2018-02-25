/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <type_traits>

namespace ac {

/**
 * Type to be used to indicate the default type where supported.
 */
struct use_default {};

template <class T, class Default>
struct deduce_type : std::conditional<std::is_same_v<T, use_default>, Default, T> {};

template <class T, class Default>
using deduce_type_t = typename deduce_type<T, Default>::type;

}  // namespace ac
