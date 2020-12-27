// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include <actl/geometry/algorithm/crosses/line_line_2d.hpp>

TEST_CASE("general position all kinds") {
    auto kinds = std::vector{endpoint::free, endpoint::closed, endpoint::open};
    for (endpoint s0 : kinds) {
        for (endpoint e0 : kinds) {
            for (endpoint s1 : kinds) {
                for (endpoint e1 : kinds) {
                    auto crosses_line = [](int offset, endpoint s, endpoint e) {
                        switch (offset) {
                            case -2:
                                return e == endpoint::free;
                            case -1:
                                return e != endpoint::open;
                            case 1:
                                return s != endpoint::open;
                            case 2:
                                return s == endpoint::free;
                            default:
                                return true;
                        }
                    };
                    for (int x : irange(-2, 3)) {
                        for (int y : irange(-2, 3)) {
                            auto h = make_any_line(point{x - 1, 0}, s0, point{x + 1, 0}, e0);
                            auto v = make_any_line(point{0, y - 1}, s1, point{0, y + 1}, e1);
                            bool expected = crosses_line(x, s0, e0) && crosses_line(y, s1, e1);
                            CHECK(expected == crosses(h, v));
                            CHECK(expected == crosses(general_position_policy{policy{}}, h, v));
                        }
                    }
                }
            }
        }
    }
}
