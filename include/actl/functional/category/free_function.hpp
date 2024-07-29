// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/meta/type_at.hpp>

namespace ac {

/// Callable traits specialized only for free functions and pointers to them.
template<class T>
struct free_function_traits {
    static constexpr bool is_free_function = false;
};

/// Concept of a free function, or a pointer to one (which are synonyms in C++).
template<class T>
concept FreeFunction = free_function_traits<T>::is_free_function;

/* free function */

template<class Return, class... Parameters>
struct free_function_traits<Return(Parameters...)> {
    using return_type = Return;

    static constexpr size_t arity = sizeof...(Parameters);

    template<size_t Index>
    using parameter_at = type_at_t<Index, Parameters...>;

    static constexpr bool accepts_variadic_arguments = false;

    static constexpr bool is_noexcept = false;

    static constexpr bool is_free_function = true;
};

template<class Return, class... Parameters>
struct free_function_traits<Return(Parameters...) noexcept>
    : free_function_traits<Return(Parameters...)> {
    static constexpr bool is_noexcept = true;
};

template<class Return, class... Parameters>
struct free_function_traits<Return(Parameters..., ...)>
    : free_function_traits<Return(Parameters...)> {
    static constexpr bool accepts_variadic_arguments = true;
};

template<class Return, class... Parameters>
struct free_function_traits<Return(Parameters..., ...) noexcept>
    : free_function_traits<Return(Parameters...)> {
    static constexpr bool accepts_variadic_arguments = true;

    static constexpr bool is_noexcept = true;
};

/* free function pointer */

template<class Return, class... Parameters>
struct free_function_traits<Return (*)(Parameters...)>
    : free_function_traits<Return(Parameters...)> {};

template<class Return, class... Parameters>
struct free_function_traits<Return (*)(Parameters...) noexcept>
    : free_function_traits<Return(Parameters...) noexcept> {};

template<class Return, class... Parameters>
struct free_function_traits<Return (*)(Parameters..., ...)>
    : free_function_traits<Return(Parameters..., ...)> {};

template<class Return, class... Parameters>
struct free_function_traits<Return (*)(Parameters..., ...) noexcept>
    : free_function_traits<Return(Parameters..., ...) noexcept> {};

} // namespace ac
