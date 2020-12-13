// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <cstdint>

namespace ac::io {

using flag_t = uint32_t;

namespace flag {

enum : flag_t {
    boolalpha = 1 << 0,
    uppercase = 1 << 1,
    showbase = 1 << 2,
    showpos = 1 << 3,
    showpoint = 1 << 4,
    fixed = 1 << 5,
    scientific = 1 << 6,
    hexfloat = 1 << 7
};

}  // namespace flag

namespace groups {

enum : flag_t { floatfield };

}  // namespace groups

const flag_t group_bits[] = {flag::fixed | flag::scientific | flag::hexfloat};

}  // namespace ac::io
