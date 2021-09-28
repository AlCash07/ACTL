#include <actl/container/conversion/from_tuple.hpp>
#include "test.hpp"

struct S
{};

using tuple_ic = std::tuple<int, char>;

/* tuples with the same number of elements */
static_assert(ac::can_convert_to_v<tuple_ic, std::tuple<char, int>>);
static_assert(noexcept(ac::convert_to<tuple_ic>(std::tuple<char, int>{4, 2})));
static_assert(
    tuple_ic{4, 2} == ac::convert_to<tuple_ic>(std::tuple<char, int>{4, 2}));
static_assert(!ac::can_convert_to_v<tuple_ic, std::tuple<int, S>>);
static_assert(!ac::can_convert_to_v<tuple_ic, std::tuple<S, char>>);

/* can convert from a prefix tuple */
static_assert(ac::can_convert_to_v<tuple_ic, std::tuple<int>>);
static_assert(noexcept(ac::convert_to<tuple_ic>(std::tuple<int>{4})));
static_assert(tuple_ic{4, 0} == ac::convert_to<tuple_ic>(std::tuple<int>{4}));
static_assert(!ac::can_convert_to_v<std::tuple<int>, tuple_ic>);

/* tuple with a single element */
static_assert(ac::can_convert_to_v<int, std::tuple<int>>);
static_assert(noexcept(ac::convert_to<int>(std::tuple<int>{4})));
static_assert(4 == ac::convert_to<int>(std::tuple<int>{4}));

/* interoperability with the first element */
static_assert(ac::can_convert_to_v<tuple_ic, int>);
static_assert(noexcept(ac::convert_to<tuple_ic>(4)));
static_assert(tuple_ic{4, 0} == ac::convert_to<tuple_ic>(4));
static_assert(!ac::can_convert_to_v<tuple_ic, S>);
static_assert(!ac::can_convert_to_v<int, tuple_ic>);

TEST_CASE("tuple to vector")
{
    static_assert(ac::can_convert_to_v<std::vector<int>, tuple_ic>);
    static_assert(!noexcept(ac::convert_to<std::vector<int>>(tuple_ic{4, 2})));
    CHECK(
        std::vector{4, 2} == ac::convert_to<std::vector<int>>(tuple_ic{4, 2}));
}
