// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/meta/type_traits.hpp>
#include "test.hpp"

static_assert(std::is_same_v<int, add_const_if_t<false, int>>);
static_assert(std::is_same_v<const int, add_const_if_t<true, int>>);

static_assert(std::is_same_v<int, remove_cvref_t<volatile const int&>>);

static_assert(std::is_same_v<
              const std::integral_constant<size_t, 2>,
              decltype(size_c<2>)>);

static_assert(is_one_of_v<int, int>);
static_assert(is_one_of_v<int, double, int, float>);
static_assert(!is_one_of_v<int, const int, int&, int*>);

static_assert(are_same_v<int>);
static_assert(are_same_v<int, int>);
static_assert(are_same_v<int, int, int>);
static_assert(are_same_v<int, int, int, int>);
static_assert(!are_same_v<int*, int, int, int>);
static_assert(!are_same_v<int, int, int&, int>);
static_assert(!are_same_v<int, int, int, double>);

static_assert(ac::is_equality_comparable_v<int>);
static_assert(!ac::is_equality_comparable_v<void>);
