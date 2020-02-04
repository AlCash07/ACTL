/***************************************************************************************************
 * Copyright 2019 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <cstdint>

namespace ac::io {

using flag_t = uint32_t;

namespace flags {

enum : flag_t {
    boolalpha,
    showbase,
    showpos,
    uppercase,
    fixed,
    scientific,
    hexfloat,
    showpoint,
    unitbuf
};

}  // namespace flags

namespace groups {

enum : flag_t { floatfield };

}  // namespace groups

}  // namespace ac::io
