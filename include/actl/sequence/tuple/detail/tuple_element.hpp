// Copyright 2026 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/memory/empty_type/EmptyTrivial.hpp>
#include <cstddef> // for size_t

namespace ac::detail {

template<size_t Index, typename T>
struct tuple_element {
    T m_element;
};

template<size_t Index, EmptyTrivial Empty>
struct tuple_element<Index, Empty> {
    tuple_element() = default;
    constexpr tuple_element(Empty) noexcept {}
};

} // namespace ac::detail
