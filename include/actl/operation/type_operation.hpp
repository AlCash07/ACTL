// Copyright 2026 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/operation/arg.hpp>
#include <actl/operation/expression/expression.hpp>
#include <actl/preprocessor/AC_WARNING_DISABLE.hpp>
#include <actl/sequence/type_array/at.hpp>

namespace ac {

template<typename T, typename Arg>
struct Placeholder {};

AC_WARNING_DISABLE(AC_UNDEFINED_FUNCTION)
// It's correct to keep these functions undefined because
// they should be used only for result type deduction.
// Placeholders must be stripped before the actual execution.
template<typename T, typename Arg>
constexpr T eval(Placeholder<T, Arg>&&);

template<typename T, typename Arg>
constexpr T eval(Placeholder<T, Arg> const&);
AC_WARNING_ENABLE()

template<typename T>
    requires(!is_expression_v<T>)
constexpr decltype(auto) strip_placeholders(T&& t) {
    return std::forward<T>(t);
}

template<typename T, typename Arg>
constexpr auto strip_placeholders(Placeholder<T, Arg>) {
    return Arg{};
}

template<Operation Op, size_t... Is, typename... Args>
constexpr auto strip_placeholders(
    const expression_data<Op, std::index_sequence<Is...>, Args...>& expression
) {
    return ac::expression{
        expression.operation,
        strip_placeholders(std::get<Is>(expression.arguments))...
    };
}

template<template<typename> typename Trait, typename Arg>
struct TypeOperation {
    // Without this operator(), we'd have to make Placeholder an operation.
    template<typename... Args>
    constexpr auto operator()(Args&&...) const {
        return specialization<TypeOperation, raw_t<Args>...>::formula;
    }
};

template<template<typename> typename Trait, typename Arg>
inline constexpr TypeOperation<Trait, Arg> type_operation;

template<template<typename> typename Trait, typename Arg>
struct is_operation<TypeOperation<Trait, Arg>> : std::true_type {};

template<
    template<typename> typename Trait,
    size_t I,
    size_t N,
    typename... Args>
struct specialization<TypeOperation<Trait, Arg<I, N>>, Args...> {
    static_assert(N == sizeof...(Args));

    static constexpr auto formula = Placeholder<
        typename Trait<at_t<type_array<Args...>, I>>::type,
        Arg<I, N>>{};
};

} // namespace ac
