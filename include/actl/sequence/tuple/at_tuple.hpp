// Copyright 2026 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/numeric/constant_literals.hpp>
#include <actl/sequence/at.hpp>
#include <actl/sequence/tuple/TupleConcept.hpp>

namespace ac {

struct AtTuple : operation_base<AtTuple> {
    using parent = At::op;

    template<Tuple T, auto Index>
    static constexpr auto evaluate(T t, constant<Index>) {
        return std::get<size_t{Index}>(t);
    }
};
inline constexpr AtTuple at_tuple;

namespace At {
template<Tuple T, typename Index>
constexpr auto specialization(op, type_array<T, Index>) noexcept {
    return at_tuple;
}
} // namespace At

} // namespace ac
