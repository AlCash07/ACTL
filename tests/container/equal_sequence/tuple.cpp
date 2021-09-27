#include <actl/container/equal_sequence/tuple.hpp>
#include <array>

static_assert(ac::equal_sequence(std::tuple{4LL, 2}, std::array{4, 2}));
static_assert(!ac::equal_sequence(std::tuple{4LL, 2}, std::array{2, 2}));
static_assert(!ac::equal_sequence(std::tuple{4LL}, std::array{4, 2}));
