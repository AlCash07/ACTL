// Copyright 2025 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/meta/Reference.hpp>
#include <span>

namespace ac {

template<typename T, size_t Extent>
struct is_reference<std::span<T, Extent>> : std::true_type {};

} // namespace ac
