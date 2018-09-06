/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/traits/type_traits.hpp>
#include <functional>
#include <utility>

namespace ac {

template <class T>
inline constexpr size_t hash(const T& arg) {
    return std::hash<remove_cvref_t<T>>{}(arg);
}

template <class T, class... Ts>
inline constexpr size_t hash(const T& arg, const Ts&... args) {
    size_t h = hash(arg);
    return h ^ ((h << 6) + (h >> 2) + 0x9e3779b9 + hash(args...));
}

template <class T1, class T2>
inline constexpr size_t hash(const std::pair<T1, T2>& arg) {
    return ac::hash(arg.first, arg.second);
}

}  // namespace ac
