/***************************************************************************************************
 * Generalized container functions.
 ***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/traits/container_traits.hpp>
#include <algorithm>

namespace ac {

template <class C, class... Ts>
inline std::pair<typename C::iterator, bool> emplace(C& container, Ts&&... args) {
    if constexpr (is_associative_container_v<C>) {
        auto res = container.emplace(std::forward<Ts>(args)...);
        if constexpr (is_unique_associative_container_v<C>) {
            return {res.first, res.second};
        } else {
            return {res, true};
        }
    } else if constexpr (is_random_access_container_v<C>) {
        container.emplace_back(std::forward<Ts>(args)...);
        return {container.end() - 1, true};
    } else {
        return {container.emplace(container.end(), std::forward<Ts>(args)...), true};
    }
}

template <class C, class T>
inline void erase(C& container, const T& value) {
    if constexpr (is_associative_container_v<C>) {
        container.erase(value);
    } else {
        container.erase(std::remove(container.begin(), container.end(), value), container.end());
    }
}

template <class C, class T>
inline std::conditional_t<std::is_const_v<C>, typename std::remove_const_t<C>::const_iterator,
                          typename C::iterator>
find(C& container, const T& value) {
    if constexpr (is_associative_container_v<C>) {
        return container.find(value);
    } else {
        return std::find(std::begin(container), std::end(container), value);
    }
}

}  // namespace ac
