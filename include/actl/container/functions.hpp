/***************************************************************************************************
 * Generalized container functions.
 ***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/container/traits.hpp>
#include <algorithm>

namespace ac {

template <class C, class... Ts>
inline std::pair<typename C::iterator, bool> emplace(C& cont, Ts&&... args) {
    if constexpr (is_associative_container_v<C>) {
        auto res = cont.emplace(std::forward<Ts>(args)...);
        if constexpr (is_unique_associative_container_v<C>) {
            return {res.first, res.second};
        } else {
            return {res, true};
        }
    } else if constexpr (is_random_access_container_v<C>) {
        cont.emplace_back(std::forward<Ts>(args)...);
        return {cont.end() - 1, true};
    } else {
        return {cont.emplace(cont.end(), std::forward<Ts>(args)...), true};
    }
}

template <class C, class T>
inline void erase(C& cont, const T& value) {
    if constexpr (is_associative_container_v<C>) {
        cont.erase(value);
    } else {
        cont.erase(std::remove(cont.begin(), cont.end(), value), cont.end());
    }
}

template <class C, class T>
inline std::conditional_t<std::is_const_v<C>, typename std::remove_const_t<C>::const_iterator,
                          typename C::iterator>
find(C& cont, const T& value) {
    if constexpr (is_associative_container_v<C>) {
        return cont.find(value);
    } else {
        return std::find(cont.begin(), cont.end(), value);
    }
}

}  // namespace ac
