// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/numeric/math_operation.hpp>

namespace ac {

struct trigonometric_operation : scalar_operation {};

AC_MATH_OPERATION1(Cos, cos, trigonometric_operation)
AC_MATH_OPERATION1(Sin, sin, trigonometric_operation)
AC_MATH_OPERATION2(Atan2, atan2, trigonometric_operation)

} // namespace ac
