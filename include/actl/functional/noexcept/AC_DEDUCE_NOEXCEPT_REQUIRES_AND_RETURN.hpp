// Copyright 2022 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#pragma once

/// Macro for C++20 that avoids code duplication by returning the given
/// expression from the function and deducing the `noexcept` specification and
/// type requirements from it. For example, the following operator @code
/// template<typename T>
/// bool operator > (T lhs, T rhs) noexcept(noexcept(rhs < lhs))
///     requires requires { rhs < lhs; } {
///     return rhs < lhs;
/// } @endcode
/// can be rewritten as @code
/// template<typename T>
/// bool operator > (T lhs, T rhs)
///     AC_DEDUCE_NOEXCEPT_REQUIRES_AND_RETURN(rhs < lhs)
/// @endcode
/// Unlike #AC_DEDUCE_NOEXCEPT_DECLTYPE_AND_RETURN, this macro requires
/// return type specification, which can make the declaration more clear.
#define AC_DEDUCE_NOEXCEPT_REQUIRES_AND_RETURN(...) \
    noexcept(noexcept(__VA_ARGS__))                 \
        requires requires { __VA_ARGS__; }          \
    {                                               \
        return __VA_ARGS__;                         \
    }
// `requires` must be the last element of a function declarator
// https://en.cppreference.com/w/cpp/language/constraints#Requires_clauses
