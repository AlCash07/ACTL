// Copyright 2021 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/numeric/comparison/equality.hpp>
#include <actl/numeric/logic/logical_and.hpp>
#include <actl/operation/arg.hpp>
#include <actl/sequence/tuple/at_tuple.hpp>

namespace ac {

template<Tuple L, Tuple R, typename Indices>
struct equal_tuple_resolver;

template<Tuple L, Tuple R, size_t... Is>
struct equal_tuple_resolver<L, R, std::index_sequence<Is...>> {
    static constexpr auto formula =
        (... &&
         is_equal(at(l_, ac::constant<Is>{}), at(r_, ac::constant<Is>{})));
};

template<Tuple L, Tuple R>
struct specialization<IsEqual, L, R>
    : equal_tuple_resolver<L, R, tuple_indices_t<L>> {
    // TODO: consider returning constant<false> for tuples of different sizes.
    static_assert(std::tuple_size_v<L> == std::tuple_size_v<R>);
};

} // namespace ac
