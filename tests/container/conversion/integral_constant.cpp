#include <actl/container/conversion/integral_constant.hpp>
#include <actl_test/base/equal_same_type.hpp>
#include "test.hpp"

TEST_CASE("integral_constant conversion")
{
    using int_2_t = std::integral_constant<int, 2>;
    using ll_2_t = std::integral_constant<long long, 2>;

    static_assert(ac::can_convert_to_v<int, ll_2_t>);
    static_assert(ac::equal_same_type(2, ac::convert_to<int>(ll_2_t{})));

    static_assert(ac::can_convert_to_v<ll_2_t, int>);
    static_assert(ac::equal_same_type(ll_2_t{}, ac::convert_to<ll_2_t>(2)));
    CHECK_THROWS(ac::convert_to<ll_2_t>(3));

    static_assert(ac::can_convert_to_v<int_2_t, ll_2_t>);
    static_assert(
        ac::equal_same_type(int_2_t{}, ac::convert_to<int_2_t>(ll_2_t{})));
    static_assert(
        !ac::can_convert_to_v<int_2_t, std::integral_constant<int, 3>>);
}
