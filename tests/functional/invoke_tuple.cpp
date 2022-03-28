// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/functional/invoke_tuple.hpp>
#include "test.hpp"

namespace {

template <class T, int V>
struct const_op
{
    constexpr int operator()(T) const
    {
        return V;
    }
};

struct increment
{
    int v;

    constexpr void operator()(int x)
    {
        v += x;
    }
};

} // namespace

TEST_CASE("invocable_tuple")
{
    using CS = std::
        tuple<const_op<bool, 1>, const_op<bool, 2>, const_op<increment, 3>>;
    static_assert(1 == ac::invoke_first_matching(CS{}, true));
    static_assert(3 == ac::invoke_first_matching(CS{}, increment{1}));

    SECTION("invoke_all")
    {
        increment inc[3] = {{0}, {1}, {2}};
        std::tuple<increment&, increment&, increment&> c{
            inc[0], inc[1], inc[2]};
        ac::invoke_all_matching(c, 2);
        for (int i : ac::irange(3))
            CHECK(i + 2 == inc[i].v);
    }
}
