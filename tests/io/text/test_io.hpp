// Copyright 2019 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "test.hpp"
#include <actl/io/device/string.hpp>

template <class... Ts>
void test_write(const std::string& expected, Ts&&... args) {
    std::string s;
    write(io::string<io::app>{s}, std::forward<Ts>(args)...);
    CHECK(expected == s);
}

template <bool Valid = true, class T, class... Ts>
void test_read(const T& expected, const std::string& input, Ts&&... args) {
    T x;
    CHECK(Valid == read(io::string<io::in>{input}, std::forward<Ts>(args)..., x));
    if constexpr (Valid) {
        CHECK_EQUAL(expected, x);
    }
}
