// Copyright 2021 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <actl/meta/strict_common_type.hpp>
#include "test.hpp"

static_assert(std::is_same_v<int, ac::strict_common_type_t<int>>);
static_assert(std::is_same_v<int, ac::strict_common_type_t<int, int, int>>);

using Zero = std::integral_constant<int, 0>;
using One = std::integral_constant<int, 1>;
static_assert(std::is_same_v<Zero, ac::strict_common_type_t<Zero>>);
static_assert(std::is_same_v<Zero, ac::strict_common_type_t<Zero, One>>);
static_assert(std::is_same_v<One, ac::strict_common_type_t<One, Zero, Zero>>);

static_assert(std::is_same_v<int, ac::strict_common_type_t<int, Zero>>);
static_assert(std::is_same_v<int, ac::strict_common_type_t<One, int>>);
static_assert(std::is_same_v<int, ac::strict_common_type_t<int, Zero, int, One>>);
static_assert(std::is_same_v<int, ac::strict_common_type_t<Zero, int, One, int>>);
static_assert(std::is_same_v<int, ac::strict_common_type_t<One, Zero, int, int>>);
