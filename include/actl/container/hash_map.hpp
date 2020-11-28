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
#include <actl/std/utility.hpp>
#include <unordered_map>

namespace ac {

template <class K, class T, class H = hash_function<>, class E = std::equal_to<>,
          class A = std::allocator<std::pair<const K, T>>>
using hash_map = std::unordered_map<K, T, H, E, A>;

template <class K, class T, class H = hash_function<>, class E = std::equal_to<>,
          class A = std::allocator<std::pair<const K, T>>>
using hash_multimap = std::unordered_multimap<K, T, H, E, A>;

template <class K, class T, class H, class E, class A>
struct container_category<hash_map<K, T, H, E, A>> : virtual pair_associative_container_tag {};

template <class K, class T, class H, class E, class A>
struct range_traits<hash_map<K, T, H, E, A>> {
    struct is_container;
    struct is_unique;
};

template <class K, class T, class H, class E, class A>
struct container_category<hash_multimap<K, T, H, E, A>> : virtual pair_associative_container_tag {};

template <class K, class T, class H, class E, class A>
struct range_traits<hash_multimap<K, T, H, E, A>> {
    struct is_container;
};

}  // namespace ac
