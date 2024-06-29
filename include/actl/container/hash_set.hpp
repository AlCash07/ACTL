// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/io/argument/range.hpp>
#include <actl/numeric/hash.hpp>
#include <unordered_set>

namespace ac {

template<
    class T,
    class H = hash_function<>,
    class E = std::equal_to<>,
    class A = std::allocator<T>>
using hash_set = std::unordered_set<T, H, E, A>;

template<
    class T,
    class H = hash_function<>,
    class E = std::equal_to<>,
    class A = std::allocator<T>>
using hash_multiset = std::unordered_multiset<T, H, E, A>;

template<class T, class H, class E, class A>
struct range_properties<hash_set<T, H, E, A>> : default_range_properties {
    static constexpr bool is_container = true;
    static constexpr bool is_unique = true;
};

template<class T, class H, class E, class A>
struct range_properties<hash_multiset<T, H, E, A>> : default_range_properties {
    static constexpr bool is_container = true;
};

} // namespace ac
