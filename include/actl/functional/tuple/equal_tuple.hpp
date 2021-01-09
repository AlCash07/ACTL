// Copyright 2021 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/category/tuple.hpp>
#include <actl/functional/composite/composite_operation.hpp>
#include <actl/functional/scalar/comparison.hpp>

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

template <class T, class... Ts>
struct tuple_op_resolver : tuple_op_resolver<tuple_indices_t<T>, T, Ts...> {};

template <size_t... Is, class T, class U>
struct tuple_op_resolver<std::index_sequence<Is...>, T, U> {
    static_assert(std::tuple_size_v<T> == std::tuple_size_v<U>);

    template <class Composer, class Op>
    static constexpr auto resolve(Composer composer, const Op& op) {
        return composer(
            op.template resolve<std::tuple_element_t<Is, T>, std::tuple_element_t<Is, U>>()...);
    }
};

template <class... Ts, class T, class U>
struct overload<equal_t, tuple_tag<Ts...>, T, U> {
    static constexpr auto resolve(equal_t op) {
        return tuple_op_resolver<T, U>::resolve(equal_tuple, op);
    }
};

}  // namespace ac
