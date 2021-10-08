#include <actl/range/is_dynamic_range.hpp>
#include <vector>

static_assert(ac::is_dynamic_range_v<std::vector<int>>);
static_assert(ac::is_range_v<int[3]>);
static_assert(!ac::is_dynamic_range_v<int[3]>);
static_assert(!ac::is_dynamic_range_v<int>);
