// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/meta/constant_literals.hpp>
#include <actl/operation/operation/composite_operation.hpp>
#include <actl/operation/scalar/common/select.hpp>
#include <actl/operation/scalar/comparison/cmp3way.hpp>
#include <actl/operation/scalar/comparison/less.hpp>
#include <actl/operation/tuple/resolver.hpp>

namespace ac {

struct lexicographical_compare_tuple_f {
    static constexpr size_t inner_count = 0;

    template<size_t I = 0, typename Cmp3WayOps, typename L, typename R>
    static int evaluate(Cmp3WayOps const& ops, L const& l, R const& r) {
        using std::get;
        int v = get<I>(ops)(get<I>(l), get<I>(r));
        if constexpr (I + 1 == std::tuple_size_v<L>)
            return v;
        else
            return select(v == 0, evaluate<I + 1>(ops, l, r), v);
    }
};
inline constexpr operation_composer<lexicographical_compare_tuple_f>
    lexicographical_compare_tuple;

template<Tuple L, Tuple R>
struct overload<cmp3way_f, L, R> {
    static constexpr auto formula = tuple_op_resolver<L, R>::resolve_tuple(
        lexicographical_compare_tuple, cmp3way
    );
};

template<Tuple L, Tuple R>
struct overload<less_f, L, R> {
    static constexpr auto formula = cmp3way < 0_c;
};

} // namespace ac
