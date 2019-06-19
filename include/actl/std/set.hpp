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
#include <set>

namespace ac {

template <class T, class C, class A>
struct container_category<std::set<T, C, A>> : virtual sorted_associative_container_tag,
                                               virtual simple_associative_container_tag,
                                               virtual unique_associative_container_tag {};

template <class T, class C, class A>
struct container_category<std::multiset<T, C, A>> : virtual sorted_associative_container_tag,
                                                    virtual simple_associative_container_tag,
                                                    virtual multiple_associative_container_tag {};

}  // namespace ac
