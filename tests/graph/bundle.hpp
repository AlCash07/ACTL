// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/std/string.hpp>
#include <actl/std/utility.hpp>

struct bundle {
    int i;
    std::string s;

    explicit bundle() = default;

    explicit bundle(int i, std::string s) : i{i}, s{s} {};

    bool operator<(bundle const& rhs) const {
        return i < rhs.i;
    }

    bool operator==(bundle const& rhs) const {
        return i == rhs.i;
    }

    size_t hash() const {
        return ac::hash_value(i);
    }
};
