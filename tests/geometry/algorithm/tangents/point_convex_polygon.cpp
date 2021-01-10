// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <actl/geometry/algorithm/orientation/point_line.hpp>
#include <actl/geometry/algorithm/tangents/point_convex_polygon.hpp>
#include "geometry/polygons.hpp"
#include "test.hpp"

TEST_CASE("max polygon") {
    cast_before<Mul, long long> policy;
    constexpr int M = 100'000'000;
    auto poly = get_max_convex_polygon(M);
    auto random_coordinate = [&]() {
        int x = random.uniform(-10 * M, 10 * M);
        if (x >= 0 && x <= M)
            x += M;
        if (x < 0 && x >= -M)
            x -= M;
        return x;
    };
    for ([[maybe_unused]] int i : irange(2222)) {
        point<int> p{random_coordinate(), random_coordinate()};
        std::vector<decltype(poly.cbegin())> res;
        tangents(policy, p, poly, std::back_inserter(res));
        CHECK(2 == res.size());
        auto it0 = cyclic_iterator{std::as_const(poly), res[0]};
        line<int> l0{p, *it0};
        CHECK_FALSE(left_turn(policy, it0[1], l0));
        CHECK(right_turn(policy, it0[-1], l0));
        auto it1 = cyclic_iterator{std::as_const(poly), res[1]};
        line<int> l1{p, *it1};
        CHECK(left_turn(policy, it1[1], l1));
        CHECK_FALSE(right_turn(policy, it1[-1], l1));
    }
}
