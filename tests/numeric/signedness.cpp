// Copyright 2024 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#include <actl/numeric/signedness.hpp>
#include <actl_test/base/is_equal_same_type.hpp>
#include <cstdint>

static_assert(ac::is_equal_same_type(int32_t{3}, ac::to_signed(uint32_t{3})));
// to_signed doesn't change a signed type
static_assert(ac::is_equal_same_type(int{2}, ac::to_signed(int{2})));

static_assert(ac::is_equal_same_type(uint64_t{4}, ac::to_unsigned(int64_t{4})));
// to_unsigned doesn't change an unsigned type
static_assert(ac::is_equal_same_type(unsigned{2}, ac::to_unsigned(unsigned{2}))
);
