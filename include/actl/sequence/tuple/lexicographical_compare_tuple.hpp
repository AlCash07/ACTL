// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/core/if_else.hpp>
#include <actl/meta/constant_literals.hpp>
#include <actl/numeric/comparison/compare3way.hpp>
#include <actl/numeric/comparison/ordering.hpp>
#include <actl/operation/operation/composite_operation.hpp>
#include <actl/sequence/tuple/resolver.hpp>

namespace ac {

struct LexicographicalCompareTuple {
    static constexpr size_t inner_count = 0;

    template<size_t I = 0, typename Compare3WayOps, typename L, typename R>
    static int evaluate(Compare3WayOps const& ops, L const& l, R const& r) {
        using std::get;
        int v = get<I>(ops)(get<I>(l), get<I>(r));
        if constexpr (I + 1 == std::tuple_size_v<L>)
            return v;
        else
            return if_else(v == 0, evaluate<I + 1>(ops, l, r), v);
    }
};
inline constexpr operation_composer<LexicographicalCompareTuple>
    lexicographical_compare_tuple;

template<Tuple L, Tuple R>
struct overload<Compare3Way, L, R> {
    static constexpr auto formula = tuple_op_resolver<L, R>::resolve_tuple(
        lexicographical_compare_tuple, compare3way
    );
};

template<Tuple L, Tuple R>
struct overload<IsLess, L, R> {
    static constexpr auto formula = compare3way < 0_c;
};

} // namespace ac
