/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/traits/container_traits.hpp>
#include <map>

namespace ac {

template <class K, class T, class C, class A>
struct container_traits<std::map<K, T, C, A>> {
    struct category : virtual sorted_associative_container_tag,
                      virtual pair_associative_container_tag,
                      virtual unique_associative_container_tag {};

    template <class K1, class T1, class C1 = std::less<K1>,
              class A1 = rebind_allocator_t<A, std::pair<const K1, T1>>>
    using rebind = std::map<K1, T1, C1, A1>;
};

template <class K, class T, class C, class A>
struct container_traits<std::multimap<K, T, C, A>> {
    struct category : virtual sorted_associative_container_tag,
                      virtual pair_associative_container_tag,
                      virtual multiple_associative_container_tag {};

    template <class K1, class T1, class C1 = std::less<K1>,
              class A1 = rebind_allocator_t<A, std::pair<const K1, T1>>>
    using rebind = std::multimap<K1, T1, C1, A1>;
};

}  // namespace ac

SPECIALIZE_STD_RANGE_HASH(std::map)
SPECIALIZE_STD_RANGE_HASH(std::multimap)
