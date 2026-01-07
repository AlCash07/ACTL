// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/functional/traits/assemble_function.hpp>

namespace ac {

/// Concept of a free function, for example,
/// `float(int, int)`, which is the type of a function like this
/// @code
/// float divide(int x, int y);
/// @endcode
///
/// Function reference like `float(&)(int, int)` and
/// <a
/// href="https://en.cppreference.com/w/cpp/language/pointer.html#Pointers_to_functions">
/// function pointer</a> like `float(*)(int, int)`
/// also satisfy the concept, because they also support function call syntax.
///
/// @note It's impossible to declare a variable of a function type,
/// it has to be either a reference or a pointer.
template<typename T>
concept FreeFunction = function_traits<T>::category == function_category::free;

/* Implementation */

// Type qualifiers and pointer don't matter for a free function.
template<FreeFunction Function>
struct function_traits<Function const> : function_traits<Function> {};
template<FreeFunction Function>
struct function_traits<Function*> : function_traits<Function> {};

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
        using parameters_type = type_array<Parameters...>;                    \
        static constexpr bool accepts_variadic_arguments =                    \
            !AC_IS_EMPTY(VARGS);                                              \
        static constexpr bool is_noexcept = !AC_IS_EMPTY(NOEXCEPT);           \
    };                                                                        \
                                                                              \
    template<typename Return, typename... Parameters>                         \
    struct assemble_function<                                                 \
        function_category::free,                                              \
        Return,                                                               \
        type_array<Parameters...>,                                            \
        !AC_IS_EMPTY(VARGS),                                                  \
        !AC_IS_EMPTY(NOEXCEPT)> {                                             \
        using type = Return(Parameters... AC_UNPARENTHESIZED VARGS) NOEXCEPT; \
    };

AC_FF_VARGS()

#undef AC_FF_VARGS
#undef AC_FF_NOEXCEPT
#undef AC_FF_FULL

} // namespace ac
