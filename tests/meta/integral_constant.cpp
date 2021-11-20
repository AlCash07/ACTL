// Copyright 2021 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/meta/integral_constant.hpp>

static_assert(
    std::is_same_v<std::integral_constant<int, 2>, ac::int_constant<2>>);

static_assert(std::is_same_v<
              const std::integral_constant<int, 2>,
              decltype(ac::int_c<2>)>);

static_assert(
    std::is_same_v<std::integral_constant<size_t, 2>, ac::size_constant<2>>);

static_assert(std::is_same_v<
              const std::integral_constant<size_t, 2>,
              decltype(ac::size_c<2>)>);
