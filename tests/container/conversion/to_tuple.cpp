// Copyright 2022 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/container/conversion/to_tuple.hpp>
#include "container/conversion/custom_convertion.hpp"

using result_tuple = std::tuple<ac::converted<int>, int>;

static_assert(ac::can_convert_to_v<result_tuple, ac::converted<char>, char>);
static_assert(
    noexcept(ac::convert_to<result_tuple>(ac::converted<char>{4}, char{2})));
static_assert(
    result_tuple{4, 2} ==
    ac::convert_to<result_tuple>(ac::converted<char>{4}, char{2}));
