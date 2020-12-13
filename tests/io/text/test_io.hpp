// Copyright 2019 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/io/device/string.hpp>
#include <actl/test.hpp>

template <class... Ts>
inline void test_write(const std::string& expected, Ts&&... args) {
    std::string s;
    write(io::string<io::app>{s}, std::forward<Ts>(args)...);
    ASSERT_EQUAL(expected, s);
}

template <bool Valid = true, class T, class... Ts>
inline void test_read(const T& expected, const std::string& input, Ts&&... args) {
    T x;
    ASSERT_EQUAL(Valid, read(io::string<io::in>{input}, std::forward<Ts>(args)..., x));
    if constexpr (Valid) {
        ASSERT_EQUAL(expected, x);
    }
}
