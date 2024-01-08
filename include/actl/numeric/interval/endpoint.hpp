// Copyright 2022 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#pragma once

namespace ac {

enum class endpoint_kind : uint8_t
{
    open = 0,
    closed = 1,
    unbounded = 2,
};

template <class T, class Kind = endpoint_kind>
struct endpoint
{
    T value;
    Kind kind;
};

template <class Endpoint1, class Endpoint2>
constexpr bool endpoints_overlap(Endpoint1 const& begin, Endpoint2 const& end)
{
    if (begin.kind == endpoint_kind::unbounded ||
        end.kind == endpoint_kind::unbounded)
        return true;
    else if (
        begin.kind == endpoint_kind::closed &&
        end.kind == endpoint_kind::closed)
        return begin.value <= end.value;
    else
        return begin.value < end.value;
}

} // namespace ac
