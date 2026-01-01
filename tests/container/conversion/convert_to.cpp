// Copyright 2022 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/container/conversion/convert_to.hpp>
#include <actl_test/base/is_equal_same_type.hpp>
#include <tuple>

/* arithmetic conversions are supported */
static_assert(ac::can_convert_to_v<int, long long>);
static_assert(ac::is_equal_same_type(2, ac::convert_to<int>(2LL)));

/* empty parameters result in a default constructor */
static_assert(ac::can_convert_to_v<int>);
static_assert(ac::is_equal_same_type(0, ac::convert_to<int>()));
using tii = std::tuple<int, int>;
static_assert(ac::can_convert_to_v<tii>);
static_assert(ac::is_equal_same_type(tii{}, ac::convert_to<tii>()));
static_assert(!ac::can_convert_to_v<tii, int>);
