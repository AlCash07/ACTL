// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/range/traits/dependent.hpp>
#include <actl/std/vector.hpp>

static_assert(std::is_same_v<int, ac::range_value_t<int const[8]>>);

// Outer const affects reference for containers but not for pointers.
static_assert(std::is_same_v<int&, ac::range_reference_t<std::vector<int>>>);
static_assert(
    std::is_same_v<int const&, ac::range_reference_t<std::vector<int> const>>);

using VI = std::vector<int>;
static_assert(std::is_same_v<VI::iterator, ac::range_iterator_t<VI>>);
static_assert(
    std::is_same_v<VI::const_iterator, ac::range_iterator_t<VI const>>);
static_assert(std::is_same_v<int*, ac::range_iterator_t<int[8]>>);
static_assert(std::is_same_v<int const*, ac::range_iterator_t<int const[8]>>);
