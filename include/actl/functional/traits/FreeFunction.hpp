// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/preprocessor/AC_UNPARENTHESIZED.hpp>
#include <type_traits>

namespace ac {

namespace detail {
template<class T>
struct is_free_function : std::false_type {};
} // namespace detail

/// Concept of a free function.
template<class T>
concept FreeFunction = detail::is_free_function<T>::value;

namespace detail {

#define AC_FF_PARAMETERS()          \
    AC_FF_NOEXCEPT((Parameters...)) \
    AC_FF_NOEXCEPT((Parameters..., ...))

#define AC_FF_NOEXCEPT(PARAMETERS) \
    AC_FF_FULL(PARAMETERS, )       \
    AC_FF_FULL(PARAMETERS, noexcept)

#define AC_FF_FULL(PARAMETERS, NOEXCEPT)                                    \
    template<class Return, class... Parameters>                             \
    struct is_free_function<Return(AC_UNPARENTHESIZED PARAMETERS) NOEXCEPT> \
        : std::true_type {};

AC_FF_PARAMETERS()

#undef AC_FF_PARAMETERS
#undef AC_FF_NOEXCEPT
#undef AC_FF_FULL

} // namespace detail
} // namespace ac
