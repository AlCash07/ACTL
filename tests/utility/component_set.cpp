// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <actl/utility/component_set.hpp>
#include "test.hpp"

namespace {

template <class T, int V>
struct const_op {
    constexpr int operator()(T) const {
        return V;
    }
};

struct increment {
    int v;

    constexpr void operator()(int x) {
        v += x;
    }
};

}  // namespace

TEST_CASE("component_set") {
    using CS = ac::component_set<const_op<bool, 1>, const_op<bool, 2>, const_op<increment, 3>>;
    static_assert(1 == CS{}.execute_first(true));
    static_assert(3 == CS{}.execute_first(increment{1}));

    SECTION("execute_all") {
        increment inc[3] = {{0}, {1}, {2}};
        ac::component_set c{inc[0], inc[1], inc[2]};
        c.execute_all(2);
        for (int i : ac::irange(3))
            CHECK(i + 2 == inc[i].v);
    }
}
