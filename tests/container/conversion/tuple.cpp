#include <actl/container/conversion/tuple.hpp>

struct S
{};

static_assert(ac::can_convert_v<std::tuple<int, char>, std::tuple<char, int>>);
static_assert(
    std::tuple<int, char>{4, 2} ==
    ac::convert<std::tuple<int, char>>(std::tuple<char, int>{4, 2}));

static_assert(!ac::can_convert_v<std::tuple<int, char>, S>);
static_assert(!ac::can_convert_v<int, std::tuple<int, char>>);

static_assert(!ac::can_convert_v<std::tuple<int>, std::tuple<char, int>>);
static_assert(!ac::can_convert_v<std::tuple<int, char>, std::tuple<char>>);

static_assert(!ac::can_convert_v<std::tuple<int, char>, std::tuple<int, S>>);
static_assert(!ac::can_convert_v<std::tuple<int, char>, std::tuple<S, char>>);
