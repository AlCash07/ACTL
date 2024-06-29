// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/io/argument/range.hpp>
#include <actl/numeric/hash.hpp>
#include <actl/std/utility.hpp>
#include <unordered_map>

namespace ac {

template <
    class K,
    class T,
    class H = hash_function<>,
    class E = std::equal_to<>,
    class A = std::allocator<std::pair<K const, T>>>
using hash_map = std::unordered_map<K, T, H, E, A>;

template <
    class K,
    class T,
    class H = hash_function<>,
    class E = std::equal_to<>,
    class A = std::allocator<std::pair<K const, T>>>
using hash_multimap = std::unordered_multimap<K, T, H, E, A>;

template <class K, class T, class H, class E, class A>
struct range_properties<hash_map<K, T, H, E, A>> : default_range_properties {
    static constexpr bool is_container = true;
    static constexpr bool is_unique = true;
};

template <class K, class T, class H, class E, class A>
struct range_properties<hash_multimap<K, T, H, E, A>>
    : default_range_properties {
    static constexpr bool is_container = true;
};

} // namespace ac
