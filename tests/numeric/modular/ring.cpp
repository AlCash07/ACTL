// Copyright 2018 Roman Rizvanov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <actl/numeric/algorithm/binary_pow.hpp>
#include <actl/numeric/modular/ring.hpp>
#include <actl/numeric/random/random.hpp>
#include "test.hpp"

TEST_CASE("static_quotient_ring::arithmetic") {
    using ring = static_quotient_ring<18446744073709551557ull>;  // largest uint64_t prime number
    using int_mod = ring_element<ring>;
    int_mod x = ac::random.uniform(ring::mod());
    int_mod y = ac::random.uniform(ring::mod());
    int_mod z = ac::random.uniform(ring::mod() - 1) + 1;
    CHECK(x + y == y + x);
    CHECK(x + (y + z) == (x + y) + z);
    CHECK(int_mod() == x + (-x));
    CHECK(x - y == x + (-y));
    CHECK(x * z + y * z == (x + y) * z);
    CHECK(x * z - y * z == (x - y) * z);
    CHECK(x / z - y / z == (x - y) / z);
    CHECK(x == binary_pow(x, ring::mod()));
    CHECK(int_mod{1} == x / x);
    CHECK_THROWS(x / int_mod());
    CHECK_THROWS(int_modulo<1000>(347) / int_modulo<1000>(222));
    CHECK(int_modulo<256>{1} == int_modulo<256>(13) / int_modulo<256>(13));
}
