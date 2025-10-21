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
struct as_free_function;
} // namespace detail

template<class T>
concept MemberFunction = detail::as_free_function<T>::is_member_function;

namespace detail {

template<class T>
using as_free_function_t = typename as_free_function<T>::type;

// Class parameter of a member function is always a reference.
// This helper trait ensures this by adding lvalue reference
// when member function reference qualification is empty.
template<class T>
using class_t = std::conditional_t<std::is_reference_v<T>, T, T&>;

// We define all possible member function traits using a chain of macros,
// which enumerate separate properties in the following order:
// 1. variadic arguments: empty or ...
// 2. const qualifier: empty or const
// 3. volatile qualifier: empty or volatile
// 4. reference qualifier: empty, & or &&
// 5. noexcept qualifier: empty or noexcept
#define AC_MF_PARAMETERS() \
    AC_MF_CONST(())        \
    AC_MF_CONST((, ...))
// Extra () is required because of the comma inside.

#define AC_MF_CONST(VARGS)  \
    AC_MF_VOLATILE(VARGS, ) \
    AC_MF_VOLATILE(VARGS, const)

#define AC_MF_VOLATILE(VARGS, CONST) \
    AC_MF_REF(VARGS, CONST)          \
    AC_MF_REF(VARGS, CONST volatile)

#define AC_MF_REF(VARGS, CV)   \
    AC_MF_NOEXCEPT(VARGS, CV)  \
    AC_MF_NOEXCEPT(VARGS, CV&) \
    AC_MF_NOEXCEPT(VARGS, CV&&)

#define AC_MF_NOEXCEPT(VARGS, CV_REF) \
    AC_MF_FULL(VARGS, CV_REF, )       \
    AC_MF_FULL(VARGS, CV_REF, noexcept)

#define AC_MF_FULL(VARGS, CV_REF, NOEXCEPT)                                    \
    template<class Class, class Return, class... Parameters>                   \
    struct as_free_function<Return (Class::*)(Parameters... AC_UNPARENTHESIZED \
                                                  VARGS) CV_REF NOEXCEPT> {    \
        using type = Return(                                                   \
            class_t<Class CV_REF>, Parameters... AC_UNPARENTHESIZED VARGS      \
        ) NOEXCEPT;                                                            \
        static constexpr bool is_member_function = true;                       \
    };

AC_MF_PARAMETERS()

#undef AC_MF_PARAMETERS
#undef AC_MF_CONST
#undef AC_MF_VOLATILE
#undef AC_MF_REF
#undef AC_MF_NOEXCEPT
#undef AC_MF_FULL

} // namespace detail
} // namespace ac
