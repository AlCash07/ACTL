// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/category/tuple.hpp>
#include <actl/functional/composite/composite_operation.hpp>
#include <actl/functional/scalar/comparison.hpp>

namespace ac {

struct lexicographical_compare_tuple_t {
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
constexpr operation_composer<lexicographical_compare_tuple_t> lexicographical_compare_tuple;

template <class... Ts, class T, class U>
struct overload<cmp3way_t, tuple_tag<Ts...>, T, U> {
    static constexpr auto resolve(cmp3way_t op) {
        return tuple_op_resolver<T, U>::resolve(lexicographical_compare_tuple, op);
    };
};

template <class... Ts, class T, class U>
struct overload<less_t, tuple_tag<Ts...>, T, U> {
    static constexpr auto resolve(less_t) {
        return cmp3way < 0;
    }
};

}  // namespace ac
