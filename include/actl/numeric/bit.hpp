// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/assert.hpp>
#include <actl/meta/type_traits.hpp>
#include <actl/utility/index.hpp>
#include <actl/utility/use_default.hpp>
#include <cstring>

namespace ac {

template <class T = use_default, class I>
constexpr auto bit(const I& i)
{
    ACTL_ASSERT(i >= 0);
    return deduce_t<T, I>{1} << i;
}

template <class T, class I>
constexpr bool has_bit(const T& x, const I& i)
{
    ACTL_ASSERT(i >= 0);
    return (x >> i & T{1}) != 0;
}

template <class T>
constexpr bool has_bits(const T& x, const T& bits)
{
    return (x & bits) == bits;
}

template <class T>
constexpr T clear_bits(const T& x, const T& bits)
{
    return x & ~bits;
}

template <class T, class I>
constexpr T clear_bit(const T& x, const I& i)
{
    return clear_bits(x, bit<T>(i));
}

template <class T>
constexpr T set_bits(T& x, const T& mask, const T& bits)
{
    return clear_bits(x, mask) | (bits & mask);
}

// C++20 version requires std::is_trivially_constructible_v<To>.
template <
    class To,
    class From,
    enable_int_if<
        sizeof(To) == sizeof(From) && std::is_default_constructible_v<To> &&
        std::is_trivially_copyable_v<To> &&
        std::is_trivially_copyable_v<From>> = 0>
To bit_cast(const From& src) noexcept
{
    To dst;
    std::memcpy(&dst, &src, sizeof(To));
    return dst;
}

} // namespace ac
