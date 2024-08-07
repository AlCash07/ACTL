// Copyright 2019 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <cstddef>
#include <type_traits>

namespace ac {

struct hash_access {
    template<class T, class = decltype(std::declval<T const>().hash())>
    std::true_type has_hash(int);

    template<class T>
    std::false_type has_hash(...);

    template<class T>
    static constexpr size_t hash(T const& x) {
        return x.hash();
    }
};

template<class T>
    requires(decltype(hash_access{}.has_hash<T>(0))::value)
constexpr size_t hash_value(T const& x) {
    return hash_access::hash(x);
}

} // namespace ac
