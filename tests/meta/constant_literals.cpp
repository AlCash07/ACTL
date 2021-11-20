// Copyright 2021 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#include <actl/meta/constant_literals.hpp>

using namespace ac::constant_literals;

static_assert(2 == 2_c);

// maximum long long value with thousands separators
constexpr auto max_value = 18446744073709551615ULL;
static_assert(max_value == 18446744073709551615_cll);
static_assert(max_value == 18'446'744'073'709'551'615_cll);

// different bases
static_assert(110 == 110_c);
static_assert(0b110 == 0b110_c);
static_assert(0b110 == 0B110_c);
static_assert(0110 == 0110_c);
static_assert(0x110 == 0x110_c);
static_assert(0X110 == 0X110_c);

// negative numbers
static_assert(-2 == -2_c);
// We don't use the minimum long long value in this test, because its negation
// doesn't fit into long long, so `-` on the right would be applied to a value
// of unsigned type.
static_assert(-9'223'372'036'854'775'807LL == -9'223'372'036'854'775'807_cll);
