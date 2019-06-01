/***************************************************************************************************
 * Copyright 2019 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#include <actl/io/text/arithmetic/integer_write.hpp>
#include <actl/io/text/decorator/spaced.hpp>
#include <actl/test.hpp>
#include "../test_io.hpp"

using namespace ac::io;

struct point_ {
    int x, y;
};

template <class D, class F>
inline ac::index serialize(D& od, F& fmt, const point_& x, text_tag) {
    return write(od, fmt, raw{'('}, x.x, x.y, raw{')'});
}

TEST("nested") {
    spaced<text> f;
    test_write("(1, 3), (2, 4)", f, setspace{", "}, point_{1, 3}, point_{2, 4});
}
