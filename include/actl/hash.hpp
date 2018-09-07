/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <functional>
#include <utility>

namespace ac {

template <class T>
inline constexpr size_t hash_value(const T& arg) {
    return std::hash<T>{}(arg);
}

template <class T, class... Ts>
inline constexpr size_t hash_value(const T& arg, const Ts&... args) {
    size_t h = hash_value(arg);
    return h ^ ((h << 6) + (h >> 2) + 0x9e3779b9 + hash_value(args...));
}

template <class T1, class T2>
inline constexpr size_t hash_value(const std::pair<T1, T2>& arg) {
    return hash_value(arg.first, arg.second);
}

template <class R>
inline constexpr size_t hash_range(const R& range) {
    size_t res{};
    for (const auto& value : range) res = hash_value(res, value);
    return res;
}

}  // namespace ac

#define SPECIALIZE_STD_HASH(name, kind)                                                    \
namespace std {                                                                            \
                                                                                           \
template <class... Ts>                                                                     \
struct hash<name<Ts...>> {                                                                 \
    constexpr size_t operator()(const name<Ts...>& arg) const { return hash_##kind(arg); } \
};                                                                                         \
                                                                                           \
}  // namespace std

#define SPECIALIZE_STD_VALUE_HASH(name) SPECIALIZE_STD_HASH(name, value)
#define SPECIALIZE_STD_RANGE_HASH(name) SPECIALIZE_STD_HASH(name, range)

SPECIALIZE_STD_VALUE_HASH(std::pair)
