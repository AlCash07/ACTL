/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/container/traits.hpp>
#include <actl/io/range.hpp>
#include <actl/numeric/hash.hpp>
#include <unordered_set>

namespace ac {

template <class T, class H = hash_function<T>, class E = std::equal_to<>,
          class A = std::allocator<T>>
using hash_set = std::unordered_set<T, H, E, A>;

template <class T, class H = hash_function<T>, class E = std::equal_to<>,
          class A = std::allocator<T>>
using hash_multiset = std::unordered_multiset<T, H, E, A>;

template <class T, class H, class E, class A>
struct container_category<hash_set<T, H, E, A>> : virtual simple_associative_container_tag,
                                                  virtual unique_associative_container_tag {};

template <class T, class H, class E, class A>
struct container_category<hash_multiset<T, H, E, A>>
    : virtual simple_associative_container_tag, virtual multiple_associative_container_tag {};

}  // namespace ac
