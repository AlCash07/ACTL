// Copyright 2026 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#include <actl/numeric/arithmetic/additive/all.hpp>
#include <actl/operation/arg.hpp>
#include <actl/operation/higher_order_function/strip_placeholders.hpp>

/* strip_placeholders replaces placeholders with associated Arg operations */
constexpr ac::Placeholder<int, ac::Arg<0, 1>> raw_placeholder;
static_assert(
    std::is_same_v<ac::Arg<0, 1>, decltype(strip_placeholders(raw_placeholder))>
);
static_assert(std::is_same_v<int&&, decltype(ac::strip_placeholders(0))>);

constexpr auto expression_with_placeholders = ac::subtract(
    ac::add(1, ac::Placeholder<int, ac::Arg<0, 2>>{}),
    ac::Placeholder<int, ac::Arg<1, 2>>{}
);
static_assert(std::is_same_v<
              decltype(1 + ac::l_ - ac::r_),
              decltype(strip_placeholders(expression_with_placeholders))>);
