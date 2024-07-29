// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/functional/category/free_function.hpp>
#include <actl/preprocessor/AC_UNPARENTHESIZED.hpp>
#include <type_traits>

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

// Class parameter of a member function is always a reference.
// This helper trait ensures this by adding lvalue reference
// when member function reference qualification is empty.
template<class T>
using class_t = std::conditional_t<std::is_reference_v<T>, T, T&>;

} // namespace detail

#define DEFINE_MEMBER_FUNTIONS_TRAITS_1(PARAMETERS) \
    DEFINE_MFT_2(PARAMETERS, )                      \
    DEFINE_MFT_2(PARAMETERS, const)

#define DEFINE_MFT_2(PARAMETERS, CONST) \
    DEFINE_MFT_3(PARAMETERS, CONST)     \
    DEFINE_MFT_3(PARAMETERS, CONST volatile)

#define DEFINE_MFT_3(PARAMETERS, CV) \
    DEFINE_MFT_4(PARAMETERS, CV)     \
    DEFINE_MFT_4(PARAMETERS, CV&)    \
    DEFINE_MFT_4(PARAMETERS, CV&&)

#define DEFINE_MFT_4(PARAMETERS, CV_REF) \
    DEFINE_MFT_5(PARAMETERS, CV_REF, )   \
    DEFINE_MFT_5(PARAMETERS, CV_REF, noexcept)

#define DEFINE_MFT_5(PARAMETERS, CV_REF, NOEXCEPT)                         \
    template<class Class, class Return, class... Parameters>               \
    struct member_function_traits<                                         \
        Return (Class::*)(AC_UNPARENTHESIZED PARAMETERS) CV_REF NOEXCEPT>  \
        : detail::member_as_free_fn<Return(                                \
              detail::class_t<Class CV_REF>, AC_UNPARENTHESIZED PARAMETERS \
          ) NOEXCEPT> {};

// We define all possible member function traits using a chain of macros,
// which enumerate separate properties in the following order:
// 1. variadic arguments: empty or ...
// 2. const qualifier: empty or const
// 3. volatile qualifier: empty or volatile
// 4. reference qualifier: empty, & or &&
// 5. noexcept qualifier: empty or noexcept
DEFINE_MEMBER_FUNTIONS_TRAITS_1((Parameters...))
// Extra () is required because of the comma inside.
DEFINE_MEMBER_FUNTIONS_TRAITS_1((Parameters..., ...))

#undef DEFINE_MEMBER_FUNTIONS_TRAITS_1
#undef DEFINE_MFT_2
#undef DEFINE_MFT_3
#undef DEFINE_MFT_4
#undef DEFINE_MFT_5

} // namespace ac
