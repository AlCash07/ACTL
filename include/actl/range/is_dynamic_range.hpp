// Copyright 2022 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/category/range.hpp>
#include <actl/meta/static_size.hpp>

namespace ac {

template <class T>
inline constexpr bool is_dynamic_range_v =
    is_range_v<T>&& static_size_v<std::remove_reference_t<T>> == dynamic_size;

} // namespace ac