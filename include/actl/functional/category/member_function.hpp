// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/functional/category/free_function.hpp>

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

#define DEFINE_MEMBER_FUNCTION_TRAITS_5(CONST, REF, REF_ACTUAL, NOEXCEPT)     \
    template<class Class, class Return, class... Params>                      \
    struct member_function_traits<Return (Class::*)(Params...)                \
                                      CONST REF NOEXCEPT>                     \
        : detail::member_as_free_fn<Return(Class CONST REF_ACTUAL, Params...) \
                                        NOEXCEPT> {};

// We use different REF and REF_ACTUAL,
// because empty member function reference qualification
// actually implies lvalue reference for the class,
// see DEFINE_MEMBER_FUNCTION_TRAITS_2.
#define DEFINE_MEMBER_FUNCTION_TRAITS_4(CONST, REF, REF_ACTUAL) \
    DEFINE_MEMBER_FUNCTION_TRAITS_5(CONST, REF, REF_ACTUAL, )   \
    DEFINE_MEMBER_FUNCTION_TRAITS_5(CONST, REF, REF_ACTUAL, noexcept)

#define DEFINE_MEMBER_FUNCTION_TRAITS_2(CONST)   \
    DEFINE_MEMBER_FUNCTION_TRAITS_4(CONST, , &)  \
    DEFINE_MEMBER_FUNCTION_TRAITS_4(CONST, &, &) \
    DEFINE_MEMBER_FUNCTION_TRAITS_4(CONST, &&, &&)

// We define all possible member function traits using a chain of macros,
// which enumerate separate properties in the following order:
// 2. const qualifier: empty or const
// 4. reference qualifier: empty, & or &&
// 5. noexcept qualifier: empty or noexcept
DEFINE_MEMBER_FUNCTION_TRAITS_2()
DEFINE_MEMBER_FUNCTION_TRAITS_2(const)

} // namespace ac
