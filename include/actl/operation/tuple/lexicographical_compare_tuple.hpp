// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/operation/core/composite_operation.hpp>
#include <actl/operation/scalar/common/constants.hpp>
#include <actl/operation/scalar/common/select.hpp>
#include <actl/operation/scalar/comparison/cmp3way.hpp>
#include <actl/operation/scalar/comparison/less.hpp>
#include <actl/operation/tuple/resolver.hpp>

namespace ac {

struct lexicographical_compare_tuple_f
{
    static constexpr index inner_count = -1;

    template <size_t I = 0, class Cmp3WayOps, class T, class U>
    static int evaluate(const Cmp3WayOps& ops, const T& lhs, const U& rhs)
    {
        using std::get;
        int v = get<I>(ops)(get<I>(lhs), get<I>(rhs));
        if constexpr (I + 1 == std::tuple_size_v<T>)
            return v;
        else
            return select(v == 0, evaluate<I + 1>(ops, lhs, rhs), v);
    }
};
inline constexpr operation_composer<lexicographical_compare_tuple_f>
    lexicographical_compare_tuple;

template <class T, class U>
struct overload<cmp3way_f, tuple_tag, T, U>
{
    static constexpr auto formula = tuple_op_resolver<T, U>::resolve_tuple(
        lexicographical_compare_tuple, cmp3way);
};

template <class T, class U>
struct overload<less_f, tuple_tag, T, U>
{
    static constexpr auto formula = cmp3way < zero_;
};

} // namespace ac
