// Copyright 2019 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/io/format/text/arithmetic/integer_write.hpp>
#include <actl/io/format/text/decorator/spaced.hpp>
#include <actl/std/map.hpp>
#include <actl/std/vector.hpp>
#include "io/format/text/test_io.hpp"
#include "test.hpp"

using namespace ac::io;

template <class T>
struct pair
{
    struct is_io_tuple;
    explicit pair(T x, T y) : x{x}, y{y} {}
    T x, y;
};

template <class D, class F, class T>
bool write_final(D& od, F& fmt, pair<T> const& x)
{
    return od.write('(') && write(od, fmt, x.x, x.y) && od.write(')');
}

TEST_CASE("nested")
{
    auto f = spaced{", "sv} >>= text{};
    test_write(
        "((2, 1), (4, 3)), (1, 3)",
        f,
        pair{pair{2, 1}, pair{4, 3}},
        pair{1, 3});
}

TEST_CASE("std::vector")
{
    std::vector v{2, 4, 5, 1};
    test_write("4 2 4 5 1", spaced{} >>= text{}, v);
    test_write("4 2 4 5 1", spaced{' ', ": "} >>= text{}, v);
}

TEST_CASE("pair associative container")
{
    std::map<int, int> m{{2, 4}, {5, 1}};
    test_write("2 2 4 5 1", spaced{} >>= text{}, m);
    test_write("2 2: 4 5: 1", spaced{' ', ": "} >>= text{}, m);
}
