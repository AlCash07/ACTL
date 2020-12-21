// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "io/text/test_io.hpp"
#include <actl/io/text/parser/arithmetic.hpp>

using namespace ac::io;

TEST_CASE("read boolalpha") {
    text f;
    bool x{};
    test_read(true, "true", f, boolalpha);
    test_read(false, "false", f);
    test_read<false>(x, "fals", f);
    test_read<false>(x, "truth", f);
    test_read<false>(x, "no", f);
}

TEST_CASE("read noboolalpha") {
    text f;
    bool x{};
    test_read(true, "1", f, noboolalpha);
    test_read(false, "0", f);
    test_read<false>(x, "true", f);
}
