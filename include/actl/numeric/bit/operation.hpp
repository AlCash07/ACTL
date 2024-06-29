// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/assert.hpp>
#include <actl/utility/use_default.hpp>

namespace ac {

template <class T = use_default, class I>
constexpr auto bit(I const& i) {
    AC_ASSERT(i >= 0);
    return deduce_t<T, I>{1} << i;
}

template <class T, class I>
constexpr bool has_bit(T const& x, I const& i) {
    AC_ASSERT(i >= 0);
    return (x >> i & T{1}) != 0;
}

template <class T>
constexpr bool has_bits(T const& x, T const& bits) {
    return (x & bits) == bits;
}

template <class T>
constexpr T clear_bits(T const& x, T const& bits) {
    return x & ~bits;
}

template <class T, class I>
constexpr T clear_bit(T const& x, I const& i) {
    return clear_bits(x, bit<T>(i));
}

template <class T>
constexpr T set_bits(T& x, T const& mask, T const& bits) {
    return clear_bits(x, mask) | (bits & mask);
}

} // namespace ac
