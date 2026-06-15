// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/AC_ASSERT.hpp>
#include <actl/core/use_default.hpp>

namespace ac {

template<typename T = use_default, typename I>
constexpr auto bit(I const& i) {
    AC_ASSERT(i >= 0);
    return deduce_t<T, I>{1} << i;
}

template<typename T, typename I>
constexpr bool has_bit(T const& t, I const& i) {
    AC_ASSERT(i >= 0);
    return (t >> i & T{1}) != 0;
}

template<typename T>
constexpr bool has_bits(T const& t, T const& bits) {
    return (t & bits) == bits;
}

template<typename T>
constexpr T clear_bits(T const& t, T const& bits) {
    return t & ~bits;
}

template<typename T, typename I>
constexpr T clear_bit(T const& t, I const& i) {
    return clear_bits(t, bit<T>(i));
}

template<typename T>
constexpr T set_bits(T& t, T const& mask, T const& bits) {
    return clear_bits(t, mask) | (bits & mask);
}

} // namespace ac
