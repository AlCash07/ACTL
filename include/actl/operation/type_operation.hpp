// Copyright 2026 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/operation/arg.hpp>
#include <actl/operation/expression/expression.hpp>
#include <actl/operation/operation/operation_base.hpp>
#include <actl/sequence/type_array/at.hpp>

namespace ac {

template<typename T, typename Arg>
struct Placeholder {
    // TODO: remove this function.
    template<typename... Args>
    static constexpr T evaluate(Args&&...);

    static constexpr bool is_argument_maybe_unused(size_t index) {
        return true;
    }

    struct enable_operators;
};

template<typename T, typename Arg>
constexpr T eval(Placeholder<T, Arg>&&);

template<typename T, typename Arg>
constexpr T eval(Placeholder<T, Arg> const&);

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
    return expression.operation(
        strip_placeholders(std::get<Is>(expression.arguments))...
    );
}

template<template<typename> typename Trait, typename Arg>
struct TypeOperation : operation_base<TypeOperation<Trait, Arg>> {};

template<template<typename> typename Trait, typename Arg>
inline constexpr TypeOperation<Trait, Arg> type_operation;

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
