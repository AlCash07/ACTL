/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/traits/container_traits.hpp>
#include <unordered_set>

namespace ac {

template <class T, class H, class E, class A>
struct container_traits<std::unordered_set<T, H, E, A>> {
    struct category : virtual simple_associative_container_tag,
                      virtual unique_associative_container_tag {};

    template <class T1, class H1 = std::hash<T1>, class E1 = std::equal_to<T1>,
              class A1 = rebind_allocator_t<A, T1>>
    using rebind = std::unordered_set<T1, H1, E1, A1>;
};

template <class T, class H, class E, class A>
struct container_traits<std::unordered_multiset<T, H, E, A>> {
    struct category : virtual simple_associative_container_tag,
                      virtual multiple_associative_container_tag {};

    template <class T1, class H1 = std::hash<T1>, class E1 = std::equal_to<T1>,
              class A1 = rebind_allocator_t<A, T1>>
    using rebind = std::unordered_multiset<T1, H1, E1, A1>;
};

}  // namespace ac
