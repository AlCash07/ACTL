/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/traits/container_traits.hpp>
#include <set>

namespace ac {

template <class T, class C, class A>
struct container_traits<std::set<T, C, A>> {
    struct category : virtual sorted_associative_container_tag,
                      virtual simple_associative_container_tag,
                      virtual unique_associative_container_tag {};

    template <class T1, class C1 = std::less<T1>, class A1 = rebind_allocator_t<A, T1>>
    using rebind = std::set<T1, C1, A1>;
};

template <class T, class C, class A>
struct container_traits<std::multiset<T, C, A>> {
    struct category : virtual sorted_associative_container_tag,
                      virtual simple_associative_container_tag,
                      virtual multiple_associative_container_tag {};

    template <class T1, class C1 = std::less<T1>, class A1 = rebind_allocator_t<A, T1>>
    using rebind = std::multiset<T1, C1, A1>;
};

}  // namespace ac
