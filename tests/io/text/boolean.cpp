/***************************************************************************************************
 * Copyright 2019 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#include <actl/io/text/boolean.hpp>
#include "test_io.hpp"

using namespace ac::io;

TEST("write") {
    text_format<out> f;
    test_write("true", f, boolalpha, true);
    test_write("false", f, false);
    test_write("1", f, noboolalpha, true);
    test_write("0", f, false);
}

TEST("read") {
    text_format<in> f;
    bool x{};
    test_read<true>(true, "true", f, boolalpha);
    test_read<true>(false, "false", f);
    test_read<false>(x, "fals", f);
    test_read<false>(x, "truth", f);
    test_read<false>(x, "no", f);
    test_read<true>(true, "1", f, noboolalpha);
    test_read<true>(false, "0", f);
    test_read<false>(x, "true", f);
}
