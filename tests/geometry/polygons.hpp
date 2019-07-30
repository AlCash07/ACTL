/***************************************************************************************************
 * Copyright 2019 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/geometry/polygon.hpp>
#include <actl/std/vector.hpp>

namespace ac {

inline monotone_polygon<point<int>> get_trident_polygon() {
    return {{{-2, 0}, {-1, 0}, {0, -1}, {1, 0}, {2, 0}, {2, 4}, {1, 1}, {0, 4}, {-1, 1}, {-2, 4}},
            5};
}

inline star_polygon<point<int>> get_star_polygon() {
    return {{{0, -2}, {3, -4}, {2, 0}, {5, 2}, {1, 2}, {0, 5}, {-1, 2}, {-5, 2}, {-2, 0}, {-3, -4}},
            {0, 0}};
}

inline convex_monotone_polygon<point<int>> get_octagon() {
    using CP = convex_polygon<point<int>>;
    return CP{{-3, -1}, {-1, -3}, {1, -3}, {3, -1}, {3, 1}, {1, 3}, {-1, 3}, {-3, 1}};
}

}  // namespace ac
