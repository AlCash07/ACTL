// Copyright 2026 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#pragma once

namespace ac {

/// Checks if the expression can be evaluated at compilation time.
///
/// Expression must be wrapped into a lambda, for example,
/// @code
/// if constexpr (is_constexpr([] { Array::size(); }))
/// @endcode
///
/// @note The expression must be valid.
/// AC_REQUIRES_CONSTEXPR should be used if this isn't guaranteed.
constexpr bool is_constexpr(...) {
    return false;
}
// Implementation reference:
// https://stackoverflow.com/questions/55288555/c-check-if-statement-can-be-evaluated-constexpr
/// @cond excluded from documentation because Breathe fails here
template<typename Lambda, bool = (Lambda{}(), true)>
constexpr bool is_constexpr(Lambda) {
    return true;
}
/// @endcond

} // namespace ac

/// Checks if the expression is valid and can be evaluated at compilation time.
///
/// Expression must be dependent on a template type, for example,
/// @code
/// template<typename Array>
/// auto process_array() {
///     if constexpr (AC_REQUIRES_CONSTEXPR(Array::size()))
///         ...
/// }
/// @endcode
#define AC_REQUIRES_CONSTEXPR(...)                        \
    [] {                                                  \
        if constexpr (requires { __VA_ARGS__; })          \
            return ac::is_constexpr([] { __VA_ARGS__; }); \
        else                                              \
            return false;                                 \
    }()
