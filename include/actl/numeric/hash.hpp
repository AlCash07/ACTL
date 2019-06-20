/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/numeric/bit.hpp>
#include <actl/numeric/random/splitmix64.hpp>
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

template <class T>
inline constexpr std::enable_if_t<std::is_arithmetic_v<T>, size_t> hash_value(const T& x) {
    static_assert(sizeof(T) <= sizeof(size_t), "TODO: remove this restriction");
    size_t t{};
    std::memcpy(&t + sizeof(size_t) - sizeof(T), &x, sizeof(T));
    return randomized_hash::compute(t);
}

template <class T>
inline constexpr size_t hash_value(T* x) {
    return bit_cast<size_t>(x);
}

template <class T>
inline constexpr void hash_combine(size_t& seed, const T& x) {
    seed ^= (seed << 6) + (seed >> 2) + 0x9e3779b9 + hash_value(x);
}

template <class T, class... Ts>
inline constexpr std::enable_if_t<0 < sizeof...(Ts), size_t> hash_value(const T& x,
                                                                        const Ts&... xs) {
    size_t res = hash_value(x);
    (..., hash_combine(res, xs));
    return res;
}

template <class T>
inline std::enable_if_t<is_range_v<T>, size_t> hash_value(const T& range) {
    size_t res{};
    for (const auto& value : range) hash_combine(res, value);
    return res;
}

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

template <class T>
struct hash_function {
    constexpr size_t operator()(const T& x) const { return hash_value(x); }
};

}  // namespace ac
