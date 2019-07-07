/***************************************************************************************************
 * Copyright 2019 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#include <actl/io/text/arithmetic/integer_write.hpp>
#include <actl/io/text/decorator/spaced.hpp>
#include <actl/test.hpp>
#include "io/text/test_io.hpp"

using namespace ac::io;

template <class T>
struct pair {
    explicit pair(T x, T y) : x{x}, y{y} {}
    T x, y;
};

template <class D, class F, class T>
inline ac::index serialize(D& od, F& fmt, const pair<T>& x, text_tag) {
    return write(od, fmt, raw{'('}, x.x, x.y, raw{')'});
}

TEST("nested") {
    spaced<text> f;
    test_write("((2, 1), (4, 3)), (1, 3)", f, setspace{", "}, pair{pair{2, 1}, pair{4, 3}},
               pair{1, 3});
}

TEST("vector") {
    spaced<text> f;
    test_write("5 3 2 1", f, std::vector{5, 3, 2, 1});
}
