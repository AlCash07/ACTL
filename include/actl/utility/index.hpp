// Copyright 2019 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <cstddef>
#include <type_traits>

namespace ac {

// http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Res-subscripts
using index = std::ptrdiff_t;

template <index N>
using index_constant = std::integral_constant<index, N>;

}  // namespace ac
