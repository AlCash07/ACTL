#include <actl/meta/static_size.hpp>
#include <utility>

static_assert(ac::dynamic_size == ac::static_size_v<int>);
static_assert(2 == ac::static_size_v<std::pair<int, float>>);
static_assert(3 == ac::static_size_v<int[3]>);

struct S
{};
static_assert(ac::dynamic_size == ac::static_size_v<S>);
