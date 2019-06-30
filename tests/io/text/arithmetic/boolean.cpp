/***************************************************************************************************
 * Copyright 2019 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

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

TEST("read") {
    text f;
    bool x{};
    test_read(true, "true", f, boolalpha);
    test_read(false, "false", f);
    test_read<false>(x, "fals", f);
    test_read<false>(x, "truth", f);
    test_read<false>(x, "no", f);
    test_read(true, "1", f, noboolalpha);
    test_read(false, "0", f);
    test_read<false>(x, "true", f);
}
