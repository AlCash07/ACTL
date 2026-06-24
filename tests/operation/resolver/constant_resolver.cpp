// Copyright 2026 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#include <actl/numeric/arithmetic/additive/add_scalar.hpp>
#include <actl_test/core/is_equal_same_type.hpp>

using namespace ac::constant_literals;

static_assert(ac::is_equal_same_type(3, eval(ac::add(1, 2))));
static_assert(ac::is_equal_same_type(3, eval(ac::add(1, 2_c))));
static_assert(ac::is_equal_same_type(3, eval(ac::add(1_c, 2))));
// static_assert(ac::is_equal_same_type(3, eval(ac::add(1_c, 2_c))));
