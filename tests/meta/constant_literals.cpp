// Copyright 2021 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#include <actl/meta/constant_literals.hpp>
#include <actl_test/base/equal_same_type.hpp>

using namespace ac::constant_literals;

template<typename Int, typename Constant>
constexpr bool equal_constants(Int expected, Constant) noexcept {
    return ac::equal_same_type(expected, Constant::value);
}

static_assert(equal_constants(2, 2_c));

// maximum long long value with thousands separators
constexpr auto max_value = 18446744073709551615ULL;
static_assert(equal_constants(max_value, 18446744073709551615_cull));
static_assert(equal_constants(max_value, 18'446'744'073'709'551'615_cull));

// different bases
static_assert(equal_constants(110, 110_c));
static_assert(equal_constants(0b110, 0b110_c));
static_assert(equal_constants(0b110, 0B110_c));
static_assert(equal_constants(0110, 0110_c));
static_assert(equal_constants(0x110, 0x110_c));
static_assert(equal_constants(0X110, 0X110_c));

// negative numbers
// TODO: implement operations for constants and switch to equal_constants here.
static_assert(-2 == -2_c);
// We don't use the minimum long long value in this test, because its negation
// doesn't fit into long long, so `-` on the right would be applied to a value
// of unsigned type.
static_assert(-9'223'372'036'854'775'807LL == -9'223'372'036'854'775'807_cll);
