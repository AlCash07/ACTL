/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#include <actl/test.hpp>
#include <actl/util/component_set.hpp>

using namespace ac;

template <class T, int V>
struct const_op {
    constexpr int operator()(T) const { return V; }
};

struct increment {
    int v;

    constexpr void operator()(int& x) const { x += v; }
};

template <class... Cs>
class component_set_test : public component_set<Cs...> {
    using base_t = component_set<Cs...>;

public:
    using base_t::base_t;
    using base_t::execute_first;
    using base_t::execute_all;
};

TEST("execute_first") {
    component_set_test<const_op<bool, 1>, const_op<bool, 2>, const_op<std::string, 3>> c;
    ASSERT_EQUAL(1, c.execute_first(true));
    ASSERT_EQUAL(3, c.execute_first(std::string("3")));
}

TEST("execute_all") {
    component_set_test<increment, increment, increment> c{{1}, {2}, {4}};
    int x = 0;
    c.execute_all(x);
    ASSERT_EQUAL(7, x);
}
