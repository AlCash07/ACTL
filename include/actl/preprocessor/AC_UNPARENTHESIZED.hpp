// Copyright 2024 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#pragma once

/// Removes the outermost parenthesis from a macro parameter.
///
/// This is useful, because commas separate arguments to function macros,
/// but commas can sometimes be needed inside an argument as well.
///
/// The solution is to wrap such an argument into extra parenthesis like
/// @code MACRO((foo, bar)) @endcode
/// and use #AC_UNPARENTHESIZED in the final macro expansion.
#define AC_UNPARENTHESIZED(...) __VA_ARGS__
