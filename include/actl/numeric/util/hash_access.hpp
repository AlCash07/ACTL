/***************************************************************************************************
 * Copyright 2019 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <type_traits>

namespace ac {

struct hash_access {
    template <class T, class = decltype(std::declval<const T>().hash())>
    std::true_type has_hash(int);

    template <class T>
    std::false_type has_hash(...);

    template <class T>
    static constexpr size_t hash(const T& x) {
        return x.hash();
    }
};

template <class T, class = std::enable_if_t<decltype(hash_access{}.has_hash<T>(0))::value>>
inline constexpr size_t hash_value(const T& x) {
    return hash_access::hash(x);
}

}  // namespace ac