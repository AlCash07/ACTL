// Copyright 2025 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/std/string_view.hpp>

static_assert(ac::Reference<std::string_view>);
static_assert(ac::Reference<std::basic_string_view<char32_t>>);
