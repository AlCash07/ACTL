// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/meta/dependent.hpp>
#include <actl/std/vector.hpp>
#include "test.hpp"

// Containers preserve const for value_type but pointers don't.
static_assert(std::is_same_v<const int, ac::value_type_t<const int[8]>>);
static_assert(std::is_same_v<int, ac::value_type_t<const int* const>>);

// Outer const affects reference for containers but not for pointers.
static_assert(std::is_same_v<int&, ac::reference_t<std::vector<int>>>);
static_assert(
    std::is_same_v<const int&, ac::reference_t<const std::vector<int>>>);
static_assert(std::is_same_v<int&, ac::reference_t<int* const>>);
static_assert(std::is_same_v<const int&, ac::reference_t<const int*>>);

using VI = std::vector<int>;
static_assert(std::is_same_v<VI::iterator, ac::iterator_t<VI>>);
static_assert(std::is_same_v<VI::const_iterator, ac::iterator_t<const VI>>);
static_assert(std::is_same_v<int*, ac::iterator_t<int[8]>>);
static_assert(std::is_same_v<const int*, ac::iterator_t<int const[8]>>);
