/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/io/range.hpp>
#include <actl/std/utility.hpp>
#include <map>

namespace ac {

template <class K, class T, class C, class A>
struct container_category<std::map<K, T, C, A>> : virtual sorted_associative_container_tag,
                                                  virtual pair_associative_container_tag,
                                                  virtual unique_associative_container_tag {};

template <class K, class T, class C, class A>
struct container_category<std::multimap<K, T, C, A>> : virtual sorted_associative_container_tag,
                                                       virtual pair_associative_container_tag,
                                                       virtual multiple_associative_container_tag {
};

}  // namespace ac
