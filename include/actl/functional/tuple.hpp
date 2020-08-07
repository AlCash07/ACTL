/***************************************************************************************************
 * Copyright 2020 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/functional/scalar.hpp>

namespace ac {

template <class T>
struct is_tuple : std::false_type {};

template <class T>
using tuple_indices_t = std::make_index_sequence<std::tuple_size_v<T>>;

}  // namespace ac

namespace ac::math {

struct tuple_tag {};

template <class T>
struct category_impl<T, std::enable_if_t<is_tuple<T>::value>> {
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

template <class... Ts>
struct tuple_op {};

template <size_t... Is, class Op, class T, class U>
struct tuple_op<std::index_sequence<Is...>, Op, T, U> {
    static_assert(std::tuple_size_v<T> == std::tuple_size_v<U>);

    using type = std::tuple<
        resolved_operation_t<Op, std::tuple_element_t<Is, T>, std::tuple_element_t<Is, U>>...>;
};

template <class Op, class T, class... Ts>
using tuple_op_t = typename tuple_op<tuple_indices_t<T>, Op, T, Ts...>::type;

template <class T, class U>
struct resolved_operation<Equal, tuple_tag, T, U> {
    using type = composite_operation<EqualTuple, tuple_op_t<Equal, T, U>>;
};

struct LexicographicalCompareTuple {
    template <size_t I = 0, class Cmp3Ops, class T, class U>
    static bool evaluate(const Cmp3Ops& ops, const T& lhs, const U& rhs) {
        if constexpr (I == std::tuple_size_v<T>) {
            return false;
        } else {
            using std::get;
            int v = get<I>(ops)(get<I>(lhs), get<I>(rhs));
            return v < 0 || (v == 0 && evaluate<I + 1>(ops, lhs, rhs));
        }
    }
};
constexpr operation_composer<LexicographicalCompareTuple> lexicographical_compare_tuple;

template <class T, class U>
struct resolved_operation<Less, tuple_tag, T, U> {
    using type = composite_operation<LexicographicalCompareTuple, tuple_op_t<Cmp3Way, T, U>>;
};

}  // namespace ac::math
