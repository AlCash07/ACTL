// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include <actl/utility/component_set.hpp>

template <class T, int V>
struct const_op {
    constexpr int operator()(T) const {
        return V;
    }
};

struct increment {
    int v;

    constexpr void operator()(int& x) const {
        x += v;
    }
};

template <class... Cs>
class component_set_test : public component_set<Cs...> {
    using base_t = component_set<Cs...>;

public:
    using base_t::base_t;
    using base_t::execute_all;
    using base_t::execute_first;
};

TEST_CASE("execute_first") {
    component_set_test<const_op<bool, 1>, const_op<bool, 2>, const_op<std::string, 3>> c;
    CHECK(1 == c.execute_first(true));
    CHECK(3 == c.execute_first(std::string("3")));
}

TEST_CASE("execute_all") {
    component_set_test<increment, increment, increment> c{{1}, {2}, {4}};
    int x = 0;
    c.execute_all(x);
    CHECK(7 == x);
}
