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

#define AC_FF_PARAMETERS() \
    AC_FF_NOEXCEPT(())     \
    AC_FF_NOEXCEPT((, ...))

#define AC_FF_NOEXCEPT(VARGS) \
    AC_FF_FULL(VARGS, )       \
    AC_FF_FULL(VARGS, noexcept)

#define AC_FF_FULL(VARGS, NOEXCEPT)                                        \
    template<class Return, class... Parameters>                            \
    struct is_free_function<Return(Parameters... AC_UNPARENTHESIZED VARGS) \
                                NOEXCEPT> : std::true_type {};

AC_FF_PARAMETERS()

#undef AC_FF_PARAMETERS
#undef AC_FF_NOEXCEPT
#undef AC_FF_FULL

} // namespace detail
} // namespace ac
