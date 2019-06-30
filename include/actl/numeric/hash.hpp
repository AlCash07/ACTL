/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/numeric/bit.hpp>
#include <actl/numeric/random/splitmix64.hpp>
#include <actl/numeric/util/hash_access.hpp>
#include <actl/traits/iterator_traits.hpp>
#include <chrono>
#include <functional>

namespace ac {

// Reference: http://codeforces.com/blog/entry/62393
struct randomized_hash {
    static_assert(sizeof(size_t) <= sizeof(uint64_t), "TODO: remove this restriction");

    static const size_t seed;

    static size_t compute(size_t x) { return splitmix64::next(x + seed); }
};

inline const size_t randomized_hash::seed =
    static_cast<size_t>(std::chrono::steady_clock::now().time_since_epoch().count());

template <class T, enable_int_if<std::is_arithmetic_v<T>> = 0>
inline constexpr size_t hash_value(const T& x) {
    static_assert(sizeof(T) <= sizeof(size_t), "TODO: remove this restriction");
    size_t t{};
    std::memcpy(&t + sizeof(size_t) - sizeof(T), &x, sizeof(T));
    return randomized_hash::compute(t);
}

template <class T, enable_int_if<std::is_empty_v<T>> = 0>
inline constexpr size_t hash_value(T) {
    return 0;
}

template <class T>
inline constexpr size_t hash_value(T* x) {
    return bit_cast<size_t>(x);
}

template <class T>
inline constexpr void hash_combine(size_t& seed, const T& x) {
    seed ^= (seed << 6) + (seed >> 2) + 0x9e3779b9 + hash_value(x);
}

template <class T, class... Ts, enable_int_if<0 < sizeof...(Ts)> = 0>
inline constexpr size_t hash_value(const T& x, const Ts&... xs) {
    size_t res = hash_value(x);
    (..., hash_combine(res, xs));
    return res;
}

template <class R, enable_int_if<is_range_v<R>> = 0>
inline constexpr size_t hash_value(const R& x) {
    size_t res{};
    for (const auto& value : x) hash_combine(res, value);
    return res;
}

template <class T>
struct hash_function {
    constexpr size_t operator()(const T& x) const { return hash_value(x); }
};

}  // namespace ac
