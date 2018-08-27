/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/traits/type_traits.hpp>
#include <actl/type/use_default.hpp>

namespace ac::geometry {

struct policy {};

template <class T>
struct is_policy
    : std::bool_constant<std::is_same_v<use_default, T> || std::is_base_of_v<policy, T>> {};

template <class T, class R = void>
using disable_if_policy_t = std::enable_if_t<!is_policy<std::decay_t<T>>::value, R>;

}  // namespace ac::geometry
