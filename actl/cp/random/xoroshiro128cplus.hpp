#pragma once

namespace cp {

/**
 * Variation of Xoroshiro128+ random engine that passes TestU01 and PractRAND tests.
 * Speed on Intel(R) Core(TM) i7-4770HQ CPU @ 2.20GHz: 0.38 cycles/byte.
 */ 
struct xoroshiro128cplus {
    using result_type = uint64_t;

    result_type operator()() {
        result_type x = state[0];
        result_type y = state[1];
        result_type r = x + y;
        y ^= x;
        x = ((x << 55) ^ (x >> 9)) ^ y ^ (y << 14);
        y = (y << 36) ^ (y >> 28);
        state[0] = x;
        state[1] = y;
        return r + (x + y < y);
    }

    static constexpr result_type min() { return result_type(); }
    static constexpr result_type max() { return ~result_type(); }

protected:
    alignas(16) result_type state[2] = {0x2b2226c33510fa6, 0xf3ce5935970031d3};
};

/*
PractRAND results:
...
rng=RNG_stdin64, seed=0xbc272892
length= 2 terabytes (2^41 bytes), time= 26444 seconds
  no anomalies in 297 test result(s)
*/

}  // namespace cp
