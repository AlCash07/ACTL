/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/traits/container_traits.hpp>
#include <actl/traits/string_traits.hpp>
#include <string>

namespace ac {

template <class C, class T, class A>
struct container_category<std::basic_string<C, T, A>> : contiguous_container_tag {};

template <class C, class T, class A>
struct is_string<std::basic_string<C, T, A>, C> : std::true_type {};

}  // namespace ac
