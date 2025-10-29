// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/functional/traits/assemble_function.hpp>

namespace ac {

/// Concept of a free function, for example, `float(int, int)`, including a free
/// <a
/// href="https://en.cppreference.com/w/cpp/language/pointer.html#Pointers_to_functions">
/// function pointer</a>.
template<typename T>
concept FreeFunction = function_traits<T>::category == function_category::free;

/* Implementation */

// Type qualifiers and pointer don't matter for a free function.
template<FreeFunction Fn>
struct function_traits<Fn const> : function_traits<Fn> {};
template<FreeFunction Fn>
struct function_traits<Fn*> : function_traits<Fn> {};

#define AC_FF_VARGS() \
    AC_FF_NOEXCEPT()  \
    AC_FF_NOEXCEPT((, ...))
// Extra parenthesis () are required because of the comma inside.

#define AC_FF_NOEXCEPT(VARGS) \
    AC_FF_FULL(VARGS, )       \
    AC_FF_FULL(VARGS, noexcept)

#define AC_FF_FULL(VARGS, NOEXCEPT)                                           \
    template<typename Return, typename... Parameters>                         \
    struct function_traits<Return(Parameters... AC_UNPARENTHESIZED VARGS)     \
                               NOEXCEPT> {                                    \
        static constexpr auto category = function_category::free;             \
        using return_type = Return;                                           \
        using parameters_type = type_list<Parameters...>;                     \
        static constexpr bool accepts_variadic_arguments =                    \
            !AC_IS_EMPTY(VARGS);                                              \
        static constexpr bool is_noexcept = !AC_IS_EMPTY(NOEXCEPT);           \
    };                                                                        \
                                                                              \
    template<typename Return, typename... Parameters>                         \
    struct assemble_function<                                                 \
        function_category::free,                                              \
        Return,                                                               \
        type_list<Parameters...>,                                             \
        !AC_IS_EMPTY(VARGS),                                                  \
        !AC_IS_EMPTY(NOEXCEPT)> {                                             \
        using type = Return(Parameters... AC_UNPARENTHESIZED VARGS) NOEXCEPT; \
    };

AC_FF_VARGS()

#undef AC_FF_VARGS
#undef AC_FF_NOEXCEPT
#undef AC_FF_FULL

} // namespace ac
