// Copyright 2021 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/operation/core/composite_operation.hpp>
#include <actl/operation/scalar/comparison/equal.hpp>
#include <actl/operation/tuple/resolver.hpp>

namespace ac {

struct equal_tuple_f
{
    static constexpr index inner_count = -1;

    template <class EqualOps, class T, class U, size_t... Is>
    static bool eval(
        const EqualOps& ops,
        const T& lhs,
        const U& rhs,
        std::index_sequence<Is...>)
    {
        using std::get;
        return (... && get<Is>(ops)(get<Is>(lhs), get<Is>(rhs)));
    }

    template <class EqualOps, class T, class U>
    static bool evaluate(const EqualOps& ops, const T& lhs, const U& rhs)
    {
        return eval(ops, lhs, rhs, tuple_indices_t<T>{});
    }
};
inline constexpr operation_composer<equal_tuple_f> equal_tuple;

template <class T, class U>
struct overload<equal_f, tuple_tag, T, U>
{
    static constexpr auto formula =
        tuple_op_resolver<T, U>::resolve_tuple(equal_tuple, equal);
};

} // namespace ac
