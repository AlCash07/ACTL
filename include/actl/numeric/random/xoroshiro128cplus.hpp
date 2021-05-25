// Copyright 2018 Roman Rizvanov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <cstdint>

namespace ac {

/// Variation of Xoroshiro128+ random engine that passes TestU01 and PractRAND
/// tests. Speed on Intel(R) Core(TM) i7-4770HQ CPU @ 2.20GHz: 0.38 cycles/byte.
struct xoroshiro128cplus
{
    using result_type = uint64_t;

    uint64_t operator()()
    {
        uint64_t x = state[0];
        uint64_t y = state[1];
        uint64_t r = x + y;
        y ^= x;
        x = ((x << 55) ^ (x >> 9)) ^ y ^ (y << 14);
        y = (y << 36) ^ (y >> 28);
        state[0] = x;
        state[1] = y;
        return r + (x + y < y);
    }

    static constexpr uint64_t min()
    {
        return uint64_t{};
    }

    static constexpr uint64_t max()
    {
        return ~uint64_t{};
    }

protected:
    alignas(16) uint64_t state[2] = {0x2b2226c33510fa6, 0xf3ce5935970031d3};
};

/*
PractRAND results:
...
rng=RNG_stdin64, seed=0xbc272892
length= 2 terabytes (2^41 bytes), time= 26444 seconds
  no anomalies in 297 test result(s)
*/

} // namespace ac
