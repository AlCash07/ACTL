#include <actl/container/conversion/convert_to.hpp>
#include <actl_test/base/equal_same_type.hpp>

static_assert(ac::can_convert_to_v<int, long long>);
static_assert(ac::equal_same_type(2, ac::convert_to<int>(2LL)));

using two_t = std::integral_constant<int, 2>;
static_assert(ac::can_convert_to_v<int, two_t>);
static_assert(ac::equal_same_type(2, ac::convert_to<int>(two_t{})));
