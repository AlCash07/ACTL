/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/io/range.hpp>
#include <set>

namespace ac {

template <class T, class C, class A>
struct container_category<std::set<T, C, A>> : virtual simple_associative_container_tag {};

template <class T, class C, class A>
struct range_traits<std::set<T, C, A>> {
    struct is_container;
    struct is_sorted;
    struct is_unique;
};

template <class T, class C, class A>
struct container_category<std::multiset<T, C, A>> : virtual simple_associative_container_tag {};

template <class T, class C, class A>
struct range_traits<std::multiset<T, C, A>> {
    struct is_container;
    struct is_sorted;
};

}  // namespace ac
