// Copyright 2026 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/preprocessor/AC_WARNING_DISABLE.hpp>

namespace ac {

template<typename T, typename Arg>
struct Placeholder {};

AC_WARNING_DISABLE(AC_UNDEFINED_FUNCTION)
// It's correct to keep these functions undefined because
// they should be used only for result type deduction.
// Placeholders must be stripped before the actual evaluation.
template<typename T, typename Arg>
constexpr T eval(Placeholder<T, Arg>&&);

template<typename T, typename Arg>
constexpr T eval(Placeholder<T, Arg> const&);
AC_WARNING_ENABLE()

} // namespace ac
