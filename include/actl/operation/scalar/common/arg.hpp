// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/operation/scalar/scalar_operation.hpp>

namespace ac {

template <index I, index N>
struct arg_f
{
    using category = operation_tag;
    using argument_category = unclassified_tag;

    struct enable_operators;

    template <class T, class... Ts>
    constexpr decltype(auto) operator()(
        [[maybe_unused]] T&& x, [[maybe_unused]] Ts&&... xs) const
    {
        static_assert(1 + sizeof...(Ts) == N || N == -1);
        if constexpr (I == 0)
            return std::forward<T>(x);
        else
            return arg_f<I - 1, (N == -1 ? -1 : N - 1)>{}(
                std::forward<Ts>(xs)...);
    }
};
template <index I, index N>
inline constexpr arg_f<I, N> arg;

inline constexpr arg_f<0, 1> x_;
inline constexpr arg_f<0, 2> lhs_;
inline constexpr arg_f<1, 2> rhs_;

} // namespace ac
