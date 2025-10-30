// Copyright 2022 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#pragma once

/// Macro that avoids code duplication by returning the given expression from
/// the function and deducing the `noexcept` specification from it.
/// For example, the following operator @code
/// template<typename T>
/// bool operator > (T l, T r) noexcept(noexcept(r < l)) {
///     return r < l;
/// } @endcode
/// can be rewritten as @code
/// template<typename T>
/// bool operator > (T l, T r)
///     AC_DEDUCE_NOEXCEPT_AND_RETURN(r < l)
/// @endcode
#define AC_DEDUCE_NOEXCEPT_AND_RETURN(...) \
    noexcept(noexcept(__VA_ARGS__)) {      \
        return __VA_ARGS__;                \
    }
