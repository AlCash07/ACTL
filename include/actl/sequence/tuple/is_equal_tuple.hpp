// Copyright 2021 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/numeric/comparison/equality_scalar.hpp>
#include <actl/numeric/logic/logical_and_scalar.hpp>
#include <actl/operation/arg.hpp>
#include <actl/sequence/tuple/at_tuple.hpp>

namespace ac {

template<size_t... Is>
constexpr auto equal_tuple_resolver(std::index_sequence<Is...>) noexcept {
    return (
        ... &&
        is_equal(at_tuple(l_, constant<Is>{}), at_tuple(r_, constant<Is>{}))
    );
}

namespace IsEqual {
template<Tuple L, Tuple R>
constexpr auto specialization(op, type_array<L, R>) noexcept {
    // TODO: consider returning constant<false> for tuples of different sizes.
    static_assert(std::tuple_size_v<L> == std::tuple_size_v<R>);
    return equal_tuple_resolver(tuple_indices_t<L>{});
}
} // namespace IsEqual

} // namespace ac
