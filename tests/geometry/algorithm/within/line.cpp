// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/geometry/algorithm/within/line.hpp>
#include "test.hpp"

TEST_CASE("all cases") {
    std::vector<point<int>> points{
        {-1, 0}, {1, 1}, {3, 2}, {5, 3}, {7, 4}, {4, 3}};
    auto test_points = [&points](line_kind kind) {
        any_line<int> l{points[1], points[3], kind};
        std::vector<bool> w;
        for (const auto& p : points)
            w.push_back(within(p, l) == within::border);
        return w;
    };
    CHECK(
        std::vector{true, true, true, true, true, false} ==
        test_points(line_kind::free));
    CHECK(
        std::vector{false, true, true, true, true, false} ==
        test_points(line_kind::closed_ray));
    CHECK(
        std::vector{false, false, true, true, true, false} ==
        test_points(line_kind::open_ray));
    CHECK(
        std::vector{false, true, true, true, false, false} ==
        test_points(line_kind::closed_segment));
    CHECK(
        std::vector{false, false, true, true, false, false} ==
        test_points(line_kind::half_open_segment));
    CHECK(
        std::vector{false, false, true, false, false, false} ==
        test_points(line_kind::open_segment));
}
