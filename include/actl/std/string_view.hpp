// Copyright 2025 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/lifetime/Reference.hpp>
#include <string_view>

namespace ac {

template<typename Char, typename Traits>
struct is_reference<std::basic_string_view<Char, Traits>> : std::true_type {};

} // namespace ac
