// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/functional/composite/composite_operation.hpp>
#include <actl/functional/scalar/comparison.hpp>
#include <actl/traits/is_tuple.hpp>

namespace ac {

template <class T>
using tuple_indices_t = std::make_index_sequence<std::tuple_size_v<T>>;

}  // namespace ac

namespace ac::math {

struct tuple_tag {};

template <class T>
struct category_impl<T, std::enable_if_t<is_tuple_v<T>>> {
    using type = tuple_tag;
};

struct EqualTuple {
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
constexpr operation_composer<EqualTuple> equal_tuple;

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

template <class T, class U>
struct overload<Equal, tuple_tag, T, U> {
    static constexpr auto resolve(Equal op) {
        return tuple_op_resolver<T, U>::resolve(equal_tuple, op);
    }
};

struct LexicographicalCompareTuple {
    template <size_t I = 0, class Cmp3WayOps, class T, class U>
    static int evaluate(const Cmp3WayOps& ops, const T& lhs, const U& rhs) {
        using std::get;
        int v = get<I>(ops)(get<I>(lhs), get<I>(rhs));
        if constexpr (I + 1 == std::tuple_size_v<T>) {
            return v;
        } else {
            return select(v == 0, evaluate<I + 1>(ops, lhs, rhs), v);
        }
    }
};
constexpr operation_composer<LexicographicalCompareTuple> lexicographical_compare_tuple;

template <class T, class U>
struct overload<Cmp3Way, tuple_tag, T, U> {
    static constexpr auto resolve(Cmp3Way op) {
        return tuple_op_resolver<T, U>::resolve(lexicographical_compare_tuple, op);
    };
};

template <class T, class U>
struct overload<Less, tuple_tag, T, U> {
    static constexpr auto resolve(Less) {
        return cmp3way < 0;
    }
};

}  // namespace ac::math
