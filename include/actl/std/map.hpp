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
struct container_category<std::map<K, T, C, A>> : virtual pair_associative_container_tag {};

template <class K, class T, class C, class A>
struct range_traits<std::map<K, T, C, A>> {
    struct is_container;
    struct is_sorted;
    struct is_unique;
};

template <class K, class T, class C, class A>
struct container_category<std::multimap<K, T, C, A>> : virtual pair_associative_container_tag {};

template <class K, class T, class C, class A>
struct range_traits<std::multimap<K, T, C, A>> {
    struct is_container;
    struct is_sorted;
};

}  // namespace ac
