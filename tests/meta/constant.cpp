// Copyright 2021 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/meta/constant.hpp>
#include <actl_test/base/equal_same_type.hpp>

static_assert(ac::equal_same_type(false, ac::constant<false>::value));
static_assert(ac::equal_same_type(2, ac::constant<2>::value));

static_assert(ac::equal_same_type(false, ac::bool_constant<false>::value));
static_assert(std::is_same_v<
              ac::constant<false> const,
              decltype(ac::bool_c<false>)>);

static_assert(ac::equal_same_type(2, ac::int_constant<2>::value));
static_assert(std::is_same_v<ac::constant<2> const, decltype(ac::int_c<2>)>);

static_assert(ac::equal_same_type(size_t{2}, ac::size_constant<2>::value));
static_assert(std::is_same_v<
              ac::size_constant<2> const,
              decltype(ac::size_c<2>)>);
