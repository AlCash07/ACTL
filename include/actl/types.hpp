/***************************************************************************************************
 * Copyright 2019 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <cstddef>

namespace ac {

// http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Res-subscripts
using index = std::ptrdiff_t;

// http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rstr-zstring
using zstring = char*;

using czstring = const char*;

}  // namespace ac
