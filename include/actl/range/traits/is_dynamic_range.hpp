// Copyright 2022 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/meta/static_size.hpp>
#include <actl/range/traits/concepts.hpp>

namespace ac {

template <class T>
inline constexpr bool is_dynamic_range_v =
    Range<T> && static_size_v<T> == dynamic_size;

} // namespace ac
