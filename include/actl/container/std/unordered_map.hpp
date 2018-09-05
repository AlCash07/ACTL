/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/traits/container_traits.hpp>
#include <unordered_map>

namespace ac {

template <class K, class T, class H, class E, class A>
struct container_traits<std::unordered_map<K, T, H, E, A>> {
    struct category : virtual pair_associative_container_tag,
                      virtual unique_associative_container_tag {};

    template <class K1, class T1, class H1 = std::hash<K1>, class E1 = std::equal_to<K1>,
              class A1 = rebind_allocator_t<A, std::pair<const K1, T1>>>
    using rebind = std::unordered_map<K1, T1, H1, E1, A1>;
};

template <class K, class T, class H, class E, class A>
struct container_traits<std::unordered_multimap<K, T, H, E, A>> {
    struct category : virtual pair_associative_container_tag,
                      virtual multiple_associative_container_tag {};

    template <class K1, class T1, class H1 = std::hash<K1>, class E1 = std::equal_to<K1>,
              class A1 = rebind_allocator_t<A, std::pair<const K1, T1>>>
    using rebind = std::unordered_multimap<K1, T1, H1, E1, A1>;
};

}  // namespace ac
