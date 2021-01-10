// Copyright 2021 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/functional/operation/composite_operation.hpp>
#include <actl/functional/scalar/comparison/equal.hpp>
#include <actl/functional/tuple/resolver.hpp>

namespace ac {

struct equal_tuple_t {
    template <class EqualOps, class T, class U, size_t... Is>
    static bool eval(const EqualOps& ops, const T& lhs, const U& rhs, std::index_sequence<Is...>) {
        using std::get;
        return (... && get<Is>(ops)(get<Is>(lhs), get<Is>(rhs)));
    }

    template <class EqualOps, class T, class U>
    static bool evaluate(const EqualOps& ops, const T& lhs, const U& rhs) {
        return eval(ops, lhs, rhs, tuple_indices_t<T>{});
    }
};
constexpr operation_composer<equal_tuple_t> equal_tuple;

template <class... Ts, class T, class U>
struct overload<equal_t, tuple_tag<Ts...>, T, U> {
    static constexpr auto resolve(equal_t op) {
        return tuple_op_resolver<T, U>::resolve(equal_tuple, op);
    }
};

}  // namespace ac
