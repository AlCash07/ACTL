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
using deduce_t = std::conditional_t<std::is_same<T, use_default>::value, Default, T>;

}  // namespace ac
