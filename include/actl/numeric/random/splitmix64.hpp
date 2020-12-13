// Source: http://xorshift.di.unimi.it/splitmix64.c
//
// Copyright 2015 Sebastiano Vigna.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <cstdint>

namespace ac {

class splitmix64 {
public:
    using result_type = uint64_t;

    static uint64_t next(uint64_t x) {
        x += 0x9e3779b97f4a7c15;
        x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
        x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
        return x ^ (x >> 31);
    }

    explicit splitmix64(uint64_t seed = {}) : state_{seed} {}

    uint64_t operator()() { return state_ = next(state_); }

private:
    uint64_t state_;
};

}  // namespace ac
