/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <string>
#include <utility>

struct bundle {
    int i;
    std::string s;

    explicit bundle() = default;
    explicit bundle(int i, std::string s) : i{i}, s{s} {};

    bool operator < (const bundle& rhs) const { return i < rhs.i; }
    bool operator == (const bundle& rhs) const { return i == rhs.i; }
};

namespace std {

template <>
struct hash<bundle> {
    auto operator()(const bundle& arg) const { return hash<int>{}(arg.i); }
};

}  // namespace std
