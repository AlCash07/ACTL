// Copyright 2022 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#pragma once

/// Macro that avoids code duplication by returning the given expression from
/// the function and deducing the `noexcept` specification and return type from
/// it. For example, the following operator @code
/// template <class T>
/// auto operator > (T lhs, T rhs) noexcept(noexcept(rhs < lhs))
///     -> decltype(rhs < lhs) {
///     return rhs < lhs;
/// } @endcode
/// can be rewritten as @code
/// template <class T>
/// auto operator > (T lhs, T rhs)
///     AC_DEDUCE_NOEXCEPT_DECLTYPE_AND_RETURN(rhs < lhs)
/// @endcode
/// This macro is different from #AC_DEDUCE_NOEXCEPT_AND_RETURN, because
/// it correctly disables a function when the expression is ill-formed (SFINAE)
/// instead of defining a function that always produces a compilation error.
#define AC_DEDUCE_NOEXCEPT_DECLTYPE_AND_RETURN(...)          \
    noexcept(noexcept(__VA_ARGS__))->decltype(__VA_ARGS__) { \
        return __VA_ARGS__;                                  \
    }
