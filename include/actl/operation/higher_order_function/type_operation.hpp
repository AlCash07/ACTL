// Copyright 2026 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/operation/arg.hpp>
#include <actl/operation/higher_order_function/placeholder.hpp>
#include <actl/operation/resolver/specialization.hpp>
#include <actl/sequence/type_array/at.hpp>

namespace ac {

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
