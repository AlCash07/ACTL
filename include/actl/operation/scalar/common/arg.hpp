// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/operation/scalar/scalar_operation.hpp>

namespace ac {

template<size_t I, size_t N>
struct arg_f {
    template<typename T, typename... Ts>
    constexpr decltype(auto) operator()(
        [[maybe_unused]] T&& x, [[maybe_unused]] Ts&&... xs
    ) const {
        static_assert(1 + sizeof...(Ts) == N);
        if constexpr (I == 0)
            return std::forward<T>(x);
        else
            return arg_f<I - 1, N - 1>{}(std::forward<Ts>(xs)...);
    }

    struct enable_operators;
};
template<size_t I, size_t N>
inline constexpr arg_f<I, N> arg;

inline constexpr arg_f<0, 1> x_;
inline constexpr arg_f<0, 2> l_;
inline constexpr arg_f<1, 2> r_;

template<size_t I, size_t N>
struct is_operation<arg_f<I, N>> : std::true_type {};

} // namespace ac
