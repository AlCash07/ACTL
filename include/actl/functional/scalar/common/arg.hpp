// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/functional/scalar/scalar_operation.hpp>

namespace ac {

template <index I, index N>
struct arg_t {
    using category = operation_tag;
    struct enable_operators;

    template <class T, class... Ts>
    constexpr decltype(auto) operator()(T&& x, Ts&&... xs) const {
        static_assert(1 + sizeof...(Ts) == N || N == -1);
        if constexpr (I == 0)
            return std::forward<T>(x);
        else
            return arg_t<I - 1, (N == -1 ? -1 : N - 1)>{}(std::forward<Ts>(xs)...);
    }
};
template <index I, index N>
constexpr arg_t<I, N> arg;

constexpr arg_t<0, 1> x_;
constexpr arg_t<0, 2> lhs_;
constexpr arg_t<1, 2> rhs_;

}  // namespace ac
