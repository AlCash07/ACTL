/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/type/use_default.hpp>
#include <actl/type_traits/type_traits.hpp>

namespace ac { namespace geometry {

struct policy {};

template <class T>
struct is_policy
    : bool_constant<std::is_same<use_default, T>::value || std::is_base_of<policy, T>::value> {};

template <class T, class R = void>
using disable_if_policy_t = std::enable_if_t<!is_policy<std::decay_t<T>>::value, R>;

}}  // namespace ac::geometry
