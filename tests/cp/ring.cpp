#include <actl/cp/ring.hpp>
#include <actl/cp/math.hpp>
#include <actl/cp/random.hpp>
#include <actl/cp/test.hpp>

using namespace cp;

TEST("static_quotient_ring:arithmetic", repeat=1000, time_limit=1) {
    using ring = static_quotient_ring<18446744073709551557ull>;  // largest uint64_t prime number
    using int_mod = ring_element<ring>;
    int_mod x = random.uniform(ring::mod());
    int_mod y = random.uniform(ring::mod());
    int_mod z = random.uniform(ring::mod() - 1) + 1;
    ASSERT_EQUAL(x + y, y + x);
    ASSERT_EQUAL(x + (y + z), (x + y) + z);
    ASSERT_EQUAL(int_mod(), x + (-x));
    ASSERT_EQUAL(x - y, x + (-y));
    ASSERT_EQUAL(x * z + y * z, (x + y) * z);
    ASSERT_EQUAL(x * z - y * z, (x - y) * z);
    ASSERT_EQUAL(x / z - y / z, (x - y) / z);
    ASSERT_EQUAL(x, math::pow(x, ring::mod()));
    ASSERT_EQUAL(1, x / x);
    ASSERT_THROWS(x / int_mod());
    ASSERT_THROWS(int_modulo<1000>(347) / int_modulo<1000>(222));
    ASSERT_EQUAL(1, int_modulo<256>(13) / int_modulo<256>(13));
}
