// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/category/range.hpp>
#include <actl/meta/rebind.hpp>
#include <actl/numeric/bit/bit_cast.hpp>
#include <actl/numeric/random/splitmix64.hpp>
#include <actl/numeric/utility/hash_access.hpp>
#include <chrono>
#include <functional>

namespace ac {

// Reference: http://codeforces.com/blog/entry/62393
struct randomized_hash
{
    static_assert(
        sizeof(size_t) <= sizeof(uint64_t), "TODO: remove this restriction");

    static size_t const seed;

    static size_t compute(size_t x)
    {
        return splitmix64::next(x + seed);
    }
};

inline size_t const randomized_hash::seed = static_cast<size_t>(
    std::chrono::steady_clock::now().time_since_epoch().count());

template <class T, enable_int_if<std::is_arithmetic_v<T>> = 0>
constexpr size_t hash_value(T const& x)
{
    static_assert(sizeof(T) <= sizeof(size_t), "TODO: remove this restriction");
    size_t t{};
    std::memcpy(
        reinterpret_cast<std::byte*>(&t) + sizeof(size_t) - sizeof(T),
        &x,
        sizeof(T));
    return randomized_hash::compute(t);
}

template <class T, enable_int_if<std::is_empty_v<T>> = 0>
constexpr size_t hash_value(T)
{
    return 0;
}

template <class T>
constexpr size_t hash_value(T* x)
{
    return bit_cast<size_t>(x);
}

template <class T>
constexpr void hash_combine(size_t& seed, T const& x)
{
    seed ^= (seed << 6) + (seed >> 2) + 0x9e3779b9 + hash_value(x);
}

template <class T, class... Ts, enable_int_if<0 < sizeof...(Ts)> = 0>
constexpr size_t hash_value(T const& x, Ts const&... xs)
{
    size_t res = hash_value(x);
    (..., hash_combine(res, xs));
    return res;
}

template <class R, enable_int_if<is_range_v<R>> = 0>
constexpr size_t hash_value(R const& x)
{
    size_t res{};
    for (auto const& value : x)
        hash_combine(res, value);
    return res;
}

template <class T = void>
struct hash_function
{
    constexpr size_t operator()(T const& x) const
    {
        return hash_value(x);
    }
};

template <>
struct hash_function<void>
{
    using transparent_key_equal = std::equal_to<>;

    template <class T>
    constexpr size_t operator()(T const& x) const
    {
        return hash_value(x);
    }
};

template <class T, class To>
struct rebind<hash_function<T>, To>
{
    using type = hash_function<>;
};

} // namespace ac
