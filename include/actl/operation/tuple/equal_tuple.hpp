// Copyright 2021 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/operation/operation/composite_operation.hpp>
#include <actl/operation/scalar/comparison/equal.hpp>
#include <actl/operation/tuple/resolver.hpp>

namespace ac {

struct equal_tuple_f {
    static constexpr size_t inner_count = 0;

    template<typename EqualOps, typename L, typename R, size_t... Is>
    static bool
    eval(EqualOps const& ops, L const& l, R const& r, std::index_sequence<Is...>) {
        using std::get;
        return (... && get<Is>(ops)(get<Is>(l), get<Is>(r)));
    }

    template<typename EqualOps, typename L, typename R>
    static bool evaluate(EqualOps const& ops, L const& l, R const& r) {
        return eval(ops, l, r, tuple_indices_t<L>{});
    }
};
inline constexpr operation_composer<equal_tuple_f> equal_tuple;

template<Tuple L, Tuple R>
struct overload<equal_f, L, R> {
    static constexpr auto formula =
        tuple_op_resolver<L, R>::resolve_tuple(equal_tuple, equal);
};

} // namespace ac
