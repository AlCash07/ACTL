// Copyright 2024 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/preprocessor/AC_UNPARENTHESIZED.hpp>
#include <type_traits>

namespace ac::detail {

template<class T>
struct as_free_function;

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
#define AC_MFT_PARAMETERS()       \
    AC_MFT_CONST((Parameters...)) \
    AC_MFT_CONST((Parameters..., ...))
// Extra () is required because of the comma inside.

#define AC_MFT_CONST(PARAMETERS)  \
    AC_MFT_VOLATILE(PARAMETERS, ) \
    AC_MFT_VOLATILE(PARAMETERS, const)

#define AC_MFT_VOLATILE(PARAMETERS, CONST) \
    AC_MFT_REF(PARAMETERS, CONST)          \
    AC_MFT_REF(PARAMETERS, CONST volatile)

#define AC_MFT_REF(PARAMETERS, CV)   \
    AC_MFT_NOEXCEPT(PARAMETERS, CV)  \
    AC_MFT_NOEXCEPT(PARAMETERS, CV&) \
    AC_MFT_NOEXCEPT(PARAMETERS, CV&&)

#define AC_MFT_NOEXCEPT(PARAMETERS, CV_REF) \
    AC_MFT_FULL(PARAMETERS, CV_REF, )       \
    AC_MFT_FULL(PARAMETERS, CV_REF, noexcept)

#define AC_MFT_FULL(PARAMETERS, CV_REF, NOEXCEPT)                            \
    template<class Class, class Return, class... Parameters>                 \
    struct as_free_function<Return (Class::*)(AC_UNPARENTHESIZED PARAMETERS) \
                                CV_REF NOEXCEPT> {                           \
        using type = Return(                                                 \
            class_t<Class CV_REF>, AC_UNPARENTHESIZED PARAMETERS             \
        ) NOEXCEPT;                                                          \
        static constexpr bool is_member_function = true;                     \
    };

AC_MFT_PARAMETERS()

#undef AC_MFT_PARAMETERS
#undef AC_MFT_CONST
#undef AC_MFT_VOLATILE
#undef AC_MFT_REF
#undef AC_MFT_NOEXCEPT
#undef AC_MFT_FULL

} // namespace ac::detail
