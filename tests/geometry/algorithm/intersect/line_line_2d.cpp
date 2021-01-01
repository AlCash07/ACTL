// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include <actl/geometry/algorithm/intersect/line_line_2d.hpp>

TEST_CASE("general position") {
    std::vector<point<double>> res;
    general_position_policy policy{geometry_policy};
    intersect(policy, segment<int>{{1, 1}, {5, 3}}, segment<int>{{1, 5}, {5, -1}},
              std::back_inserter(res));
    CHECK(1u == res.size());
    CHECK_NEAR(point{3, 2}, res[0], 1e-12);
    intersect(policy, line<int>{{1, 1}, {5, 3}}, line<int>{{0, 1}, {4, 3}},
              std::back_inserter(res));
    CHECK(1u == res.size());
}

template <ac::index N, class T, class K1, class K2>
bool equal_lines(const line<T, N, K1>& lhs, const line<T, N, K2>& rhs) {
    if (lhs.kind() != rhs.kind())
        return false;
    bool ok = lhs.begin == rhs.begin && lhs.vector == rhs.vector;
    if (end(lhs.kind()) == begin(lhs.kind())) {
        if (lhs.begin == rhs.end() && lhs.vector == -rhs.vector)
            ok = true;
    }
    return ok;
}

TEST_CASE("segments interleaving") {
    point p0{0, 0}, p1{1, 0}, p2{2, 0}, p3{3, 0};
    for (int dir0 : irange(2)) {
        for (int dir1 : irange(2)) {
            auto s0 = dir0 == 0 ? segment<int>{p0, p3} : segment<int>{p3, p0};
            auto s1 = dir1 == 0 ? segment<int>{p1, p2} : segment<int>{p2, p1};
            segment<int> res;
            CHECK(&res + 1 == intersect(default_policy, s0, s1, &res));
            CHECK(equal_lines(segment<int>{p1, p2}, res));
            s0 = dir0 == 0 ? segment<int>{p0, p2} : segment<int>{p2, p0};
            s1 = dir1 == 0 ? segment<int>{p1, p3} : segment<int>{p3, p1};
            CHECK(&res + 1 == intersect(default_policy, s0, s1, &res));
            CHECK(equal_lines(segment<int>{p1, p2}, res));
        }
    }
}

TEST_CASE("common all kinds") {
    auto kinds = std::vector{endpoint::free, endpoint::closed, endpoint::open};
    for (endpoint s0 : kinds) {
        for (endpoint e0 : kinds) {
            for (endpoint s1 : kinds) {
                for (endpoint e1 : kinds) {
                    point p0{0, 0}, p1{2, 1}, p2{4, 2}, p3{6, 3};
                    auto l0 = make_any_line(p0, s0, p2, e0);
                    auto l1 = make_any_line(p1, s1, p3, e1);
                    any_line<int> res;
                    CHECK(&res + 1, intersect(default_policy, l0, l1 == &res));
                    auto sok = s1 == endpoint::free ? std::pair{p0, s0} : std::pair{p1, s1};
                    auto eok = e0 == endpoint::free ? std::pair{p3, e1} : std::pair{p2, e0};
                    CHECK(equal_lines(make_any_line(sok.first, sok.second, eok.first, eok.second),
                                      res));
                    l0 = make_any_line(p1, s0, p2, e0);
                    l1 = make_any_line(p1, s1, p2, e1);
                    CHECK(&res + 1 == intersect(default_policy, l0, l1, &res));
                    CHECK(equal_lines(make_any_line(p1, std::max(s0, s1), p2, std::max(e0, e1)),
                                      res));
                }
            }
        }
    }
}
