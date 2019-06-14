/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/hash.hpp>
#include <actl/io/range.hpp>
#include <actl/traits/container_traits.hpp>
#include <deque>

namespace ac {

template <class T, class A>
struct container_category<std::deque<T, A>> : random_access_container_tag {};

}  // namespace ac

SPECIALIZE_STD_RANGE_HASH(std::deque)
