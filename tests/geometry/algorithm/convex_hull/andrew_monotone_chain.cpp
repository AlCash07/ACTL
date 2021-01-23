// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/geometry/algorithm/convex_hull/andrew_monotone_chain.hpp>
#include "test.hpp"

TEST_CASE("random", repeat = 10) {
    constexpr int R = 10000;
    auto gen = [&random]() { return random.uniform(-R, R); };
    std::vector<point<int>> points;
    while (points.size() < 1000) {
        point p{gen(), gen()};
        if (dot(p) <= sqr(R))
            points.push_back(p);
    }
    auto points_copy = points;
    auto hull = convex_hull(span{points_copy});
    for (auto i : irange(hull.size())) {
        line<int> l{hull[i], hull[i + 1 < hull.size() ? i + 1 : 0]};
        for (const auto& p : points) {
            CHECK_FALSE(right_turn(l, p));
        }
    }
}
