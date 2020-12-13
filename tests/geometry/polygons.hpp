// Copyright 2019 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/geometry/polygon.hpp>
#include <actl/std/vector.hpp>

namespace ac {

monotone_polygon<point<int>> get_trident_polygon() {
    return {{{-2, 0}, {-1, 0}, {0, -1}, {1, 0}, {2, 0}, {2, 4}, {1, 1}, {0, 4}, {-1, 1}, {-2, 4}},
            5};
}

star_polygon<point<int>> get_star_polygon() {
    return {{{0, -2}, {3, -4}, {2, 0}, {5, 2}, {1, 2}, {0, 5}, {-1, 2}, {-5, 2}, {-2, 0}, {-3, -4}},
            {0, 0}};
}

convex_monotone_polygon<point<int>> get_octagon() {
    using CP = convex_polygon<point<int>>;
    return CP{{-3, -1}, {-1, -3}, {1, -3}, {3, -1}, {3, 1}, {1, 3}, {-1, 3}, {-3, 1}};
}

convex_polygon<point<int>> get_max_convex_polygon(int max_coordinate) {
    auto get_max = [](int n) { return n * (n + 3) / 2; };
    convex_polygon<point<int>> poly;
    int n = 0;
    while (get_max(n + 1) <= max_coordinate) ++n;
    int y = max_coordinate - n;
    int x = y + 1;
    for (index i : irange(n)) {
        poly.emplace_back(x, -y);
        ++x;
        y -= i + 2;
    }
    for (index i : irange(n)) {
        const auto& p = poly[size_t(n - i - 1)];
        poly.emplace_back(p[0], -p[1]);
    }
    index m = 2 * n;
    for ([[maybe_unused]] index i : irange(3 * m)) {
        poly.emplace_back(perpendicular(poly[poly.size() - (size_t)m]));
    }
    return poly;
}

}  // namespace ac
