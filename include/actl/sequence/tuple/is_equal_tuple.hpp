// Copyright 2021 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/numeric/comparison/equality.hpp>
#include <actl/operation/operation/composite_operation.hpp>
#include <actl/sequence/tuple/resolver.hpp>

namespace ac {

struct IsEqualTuple {
    static constexpr size_t inner_count = 0;

    template<typename EqualOps, typename L, typename R, size_t... Is>
    static bool
    evaluate_impl(EqualOps const& ops, L const& l, R const& r, std::index_sequence<Is...>) {
        using std::get;
        return (... && get<Is>(ops)(get<Is>(l), get<Is>(r)));
    }

    template<typename EqualOps, typename L, typename R>
    static bool evaluate(EqualOps const& ops, L const& l, R const& r) {
        return evaluate_impl(ops, l, r, tuple_indices_t<L>{});
    }
};
inline constexpr operation_composer<IsEqualTuple> is_equal_tuple;

template<Tuple L, Tuple R>
struct overload<IsEqual, L, R> {
    static constexpr auto formula =
        tuple_op_resolver<L, R>::resolve_tuple(is_equal_tuple, is_equal);
};

} // namespace ac
