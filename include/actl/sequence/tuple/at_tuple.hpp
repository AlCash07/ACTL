// Copyright 2026 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/numeric/constant_literals.hpp>
#include <actl/operation/operation/operation_base.hpp>
#include <actl/sequence/tuple/TupleConcept.hpp>

namespace ac {

struct At;

struct AtTuple : operation_base<AtTuple> {
    using parent = At;

    template<Tuple T, auto Index>
    static constexpr auto evaluate(T t, constant<Index>) {
        return std::get<size_t{Index}>(t);
    }
};
inline constexpr AtTuple at_tuple;

} // namespace ac
