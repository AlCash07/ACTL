// Copyright 2022 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/container/conversion/from_tuple.hpp>
#include <actl/container/conversion/to_tuple.hpp>
#include "container/conversion/custom_convertion.hpp"
#include "test.hpp"

struct S
{};

using my_int = ac::converted<int>;
using my_char = ac::converted<char>;
using tuple_ic = std::tuple<my_int, char>;
using tuple_ci = std::tuple<my_char, int>;

/* tuples with the same number of elements */
static_assert(ac::can_convert_to_v<tuple_ic, tuple_ci>);
static_assert(ac::can_convert_to_v<tuple_ic, const tuple_ci&>);
static_assert(noexcept(ac::convert_to<tuple_ic>(tuple_ci{4, 2})));
static_assert(tuple_ic{4, 2} == ac::convert_to<tuple_ic>(tuple_ci{4, 2}));
static_assert(!ac::can_convert_to_v<tuple_ic, std::tuple<my_int, S>>);
static_assert(!ac::can_convert_to_v<tuple_ic, std::tuple<S, my_char>>);

/* interoperability with the first element */
static_assert(ac::can_convert_to_v<my_int, std::tuple<my_char>>);
static_assert(ac::can_convert_to_v<my_int, std::tuple<my_char>&>);
static_assert(noexcept(ac::convert_to<my_int>(std::tuple<my_char>{4})));
static_assert(my_int{4} == ac::convert_to<my_int>(std::tuple<my_char>{4}));

static_assert(!ac::can_convert_to_v<my_int, tuple_ic>);
static_assert(!ac::can_convert_to_v<std::tuple<my_int>, tuple_ic>);
static_assert(!ac::can_convert_to_v<tuple_ic, S>);

TEST_CASE("tuple to vector")
{
    using ivec = std::vector<int>;
    static_assert(ac::can_convert_to_v<ivec, std::tuple<int, int>>);
    static_assert(!noexcept(ac::convert_to<ivec>(std::tuple{4, 2})));
    CHECK(std::vector{4, 2} == ac::convert_to<ivec>(std::tuple{4, 2}));
}
