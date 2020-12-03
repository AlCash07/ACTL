/***************************************************************************************************
 * Copyright 2019 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#include <actl/io/text/arithmetic/integer_write.hpp>
#include <actl/io/text/decorator/braced.hpp>
#include <actl/io/text/decorator/spaced.hpp>
#include <actl/io/text/text.hpp>
#include <actl/std/all.hpp>
#include <actl/test.hpp>

#include "io/text/test_io.hpp"

using namespace ac::io;

auto sbts() { return spaced{' ', ':'} >>= braced{} >>= text_static<bit(flags::boolalpha)>{}; }

TEST("bool") { test_write("true false", sbts(), true, false); }

TEST("int") { test_write("123", sbts(), 123); }

TEST("tuple") { test_write("(3 2 1 0)", sbts(), std::tuple{3, 2, 1, 0}); }

TEST("sequence") { test_write("[3 2 1 0]", sbts(), std::vector{3, 2, 1, 0}); }

TEST("simple associative container") { test_write("{0 1 2 3}", sbts(), std::set{3, 2, 1, 0}); }

TEST("pair associative container") {
    test_write("{0:1 1:0 2:1 3:0}", sbts(), std::map<int, int>{{3, 0}, {2, 1}, {1, 0}, {0, 1}});
}

TEST("nested") {
    test_write("{[(0 1)] [(2 3) (4 5)] [(6 7)]}", sbts(),
               std::set<std::vector<std::pair<int, int>>>{{{0, 1}}, {{2, 3}, {4, 5}}, {{6, 7}}});
}
