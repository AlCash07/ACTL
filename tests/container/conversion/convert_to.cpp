#include <actl/container/conversion/convert_to.hpp>
#include <actl_test/base/equal_same_type.hpp>

/* arithmetic conversions are supported */
static_assert(ac::can_convert_to_v<int, long long>);
static_assert(ac::equal_same_type(2, ac::convert_to<int>(2LL)));

struct P3
{
    int x, y, z;

    constexpr bool operator==(const P3& rhs) const noexcept
    {
        return x == rhs.x && y == rhs.y && z == rhs.z;
    }
};

/* multiple arguments and aggregate initialization are supported */
static_assert(ac::can_convert_to_v<P3, int, int, int>);
static_assert(ac::equal_same_type(P3{5, 4, 2}, ac::convert_to<P3>(5, 4, 2)));