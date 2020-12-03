/***************************************************************************************************
 * Copyright 2019 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#include <actl/io/text/arithmetic/integer_write.hpp>
#include <actl/io/text/decorator/spaced.hpp>
#include <actl/std/map.hpp>
#include <actl/std/vector.hpp>
#include <actl/test.hpp>

#include "io/text/test_io.hpp"

using namespace ac::io;

template <class T>
struct pair {
    struct is_io_tuple;
    explicit pair(T x, T y) : x{x}, y{y} {}
    T x, y;
};

template <class D, class F, class T>
ac::index write_final(D& od, F& fmt, const pair<T>& x) {
    return od.write('(') + write(od, fmt, x.x, x.y) + od.write(')');
}

TEST("nested") {
    auto f = spaced{", "sv} >>= text{};
    test_write("((2, 1), (4, 3)), (1, 3)", f, pair{pair{2, 1}, pair{4, 3}}, pair{1, 3});
}

TEST("std::vector") {
    std::vector v{2, 4, 5, 1};
    test_write("4 2 4 5 1", spaced{} >>= text{}, v);
    test_write("4 2 4 5 1", spaced{' ', ": "} >>= text{}, v);
}

TEST("pair associative container") {
    std::map<int, int> m{{2, 4}, {5, 1}};
    test_write("2 2 4 5 1", spaced{} >>= text{}, m);
    test_write("2 2: 4 5: 1", spaced{' ', ": "} >>= text{}, m);
}
