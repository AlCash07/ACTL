// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/functional/category/free_function.hpp>
#include <actl/preprocessor/AC_UNPARENTHESIZED.hpp>

namespace ac {

/// Callable traits specialized only for member function pointers.
template<class T>
struct member_function_traits {
    static constexpr bool is_member_function = false;
};

template<class T>
concept MemberFunction = member_function_traits<T>::is_member_function;

namespace detail {

template<class Fn>
struct member_as_free_fn : free_function_traits<Fn> {
    static constexpr bool is_member_function = true;
};

} // namespace detail

#define DEFINE_MEMBER_FUNTIONS_TRAITS_1(VARARGS) \
    DEFINE_MFT_2(VARARGS, )                      \
    DEFINE_MFT_2(VARARGS, const)

#define DEFINE_MFT_2(VARARGS, CONST) \
    DEFINE_MFT_3(VARARGS, CONST, )   \
    DEFINE_MFT_3(VARARGS, CONST, volatile)

#define DEFINE_MFT_3(VARARGS, CONST, VOLATILE)   \
    DEFINE_MFT_4(VARARGS, CONST, VOLATILE, , &)  \
    DEFINE_MFT_4(VARARGS, CONST, VOLATILE, &, &) \
    DEFINE_MFT_4(VARARGS, CONST, VOLATILE, &&, &&)

// We use different REF and REF_ACTUAL,
// because empty member function reference qualification
// actually implies lvalue reference for the class,
// see DEFINE_MFT_3.
#define DEFINE_MFT_4(VARARGS, CONST, VOLATILE, REF, REF_ACTUAL) \
    DEFINE_MFT_5(VARARGS, CONST, VOLATILE, REF, REF_ACTUAL, )   \
    DEFINE_MFT_5(VARARGS, CONST, VOLATILE, REF, REF_ACTUAL, noexcept)

#define DEFINE_MFT_5(VARARGS, CONST, VOLATILE, REF, REF_ACTUAL, NOEXCEPT) \
    template<class Class, class Return, class... Parameters>              \
    struct member_function_traits<Return (Class::*)(                      \
        Parameters... AC_UNPARENTHESIZED VARARGS                          \
    ) CONST VOLATILE REF NOEXCEPT>                                        \
        : detail::member_as_free_fn<Return(                               \
              Class CONST VOLATILE REF_ACTUAL,                            \
              Parameters... AC_UNPARENTHESIZED VARARGS                    \
          ) NOEXCEPT> {};

// We define all possible member function traits using a chain of macros,
// which enumerate separate properties in the following order:
// 1. variadic arguments: empty or ...
// 2. const qualifier: empty or const
// 3. volatile qualifier: empty or volatile
// 4. reference qualifier: empty, & or &&
// 5. noexcept qualifier: empty or noexcept
DEFINE_MEMBER_FUNTIONS_TRAITS_1(())
// Extra () is required because of the comma inside.
DEFINE_MEMBER_FUNTIONS_TRAITS_1((, ...))

#undef DEFINE_MEMBER_FUNTIONS_TRAITS_1
#undef DEFINE_MFT_2
#undef DEFINE_MFT_3
#undef DEFINE_MFT_4
#undef DEFINE_MFT_5

} // namespace ac
