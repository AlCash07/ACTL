// Copyright 2019 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <actl/io/text/arithmetic/boolean.hpp>
#include "io/text/test_io.hpp"

using namespace ac::io;

TEST("write") {
    text f;
    test_write("true", f, boolalpha, true);
    test_write("false", f, false);
    test_write("1", f, noboolalpha, true);
    test_write("0", f, false);
}
