/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/io/range.hpp>
#include <actl/numeric/hash.hpp>
#include <actl/traits/container_traits.hpp>
#include <list>

namespace ac {

template <class T, class A>
struct container_category<std::list<T, A>> : sequence_container_tag {};

}  // namespace ac
