// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/functional/traits/Function.hpp>

namespace ac {

/// Concept of a class member function, often called a method.
template<class T>
concept MemberFunction =
    Function<T> && function_traits<T>::category == function_category::member;

/* Implementation */

// Type qualifiers don't matter for a member function.
template<MemberFunction Fn>
struct function_traits<Fn const> : function_traits<Fn> {};

// We define all possible member function traits using a chain of macros,
// which enumerate separate properties in the following order:
// 1. variadic arguments: empty or ...
// 2. const qualifier: empty or const
// 3. volatile qualifier: empty or volatile
// 4. reference qualifier: empty, & or &&
// 5. noexcept qualifier: empty or noexcept
#define AC_MF_VARGS() \
    AC_MF_CONST()     \
    AC_MF_CONST((, ...))
// Extra parenthesis () are required because of the comma inside.

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

namespace detail {
// Class parameter of a member function is always a reference.
// This helper trait ensures this by adding lvalue reference
// when member function reference qualification is empty.
template<class T>
using class_t = std::conditional_t<std::is_reference_v<T>, T, T&>;
} // namespace detail

// We could inherit function_traits of a corresponding free function here,
// but that would create an unnecessary template instantiation.
#define AC_MF_FULL(VARGS, CV_REF, NOEXCEPT)                                   \
    template<class Class, class Return, class... Parameters>                  \
    struct function_traits<Return (Class::*)(Parameters... AC_UNPARENTHESIZED \
                                                 VARGS) CV_REF NOEXCEPT> {    \
        static constexpr auto category = function_category::member;           \
        using return_type = Return;                                           \
        using parameter_types =                                               \
            type_list<detail::class_t<Class CV_REF>, Parameters...>;          \
        static constexpr bool accepts_variadic_arguments =                    \
            !AC_IS_EMPTY(VARGS);                                              \
        static constexpr bool is_noexcept = !AC_IS_EMPTY(NOEXCEPT);           \
    };

AC_MF_VARGS()

#undef AC_MF_VARGS
#undef AC_MF_CONST
#undef AC_MF_VOLATILE
#undef AC_MF_REF
#undef AC_MF_NOEXCEPT
#undef AC_MF_FULL

} // namespace ac
