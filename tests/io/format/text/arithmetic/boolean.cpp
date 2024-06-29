// Copyright 2019 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/io/format/text/arithmetic/boolean.hpp>
#include "io/format/text/test_io.hpp"

using namespace ac::io;

TEST_CASE("boolean write") {
    text f;
    test_write("true", f, boolalpha, true);
    test_write("false", f, false);
    test_write("1", f, noboolalpha, true);
    test_write("0", f, false);
}
