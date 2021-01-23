// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/io/device/buffered.hpp>
#include <actl/io/device/memory.hpp>
#include <actl/io/format/unit_flush.hpp>
#include "test.hpp"

using namespace ac;
using namespace ac::io;

TEST_CASE("unit_flush") {
    char array[1] = {};
    buffered<memory<bin | io::out>, char[2]> od{array};
    write(od, unit_flush{}, 'x');
    CHECK(array[0] == 'x');
}
