// Copyright 2024 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#include <actl/functional/overloaded.hpp>
#include <variant>

namespace ac {

using int_or_float = std::variant<int, float>;

constexpr auto has_int = overloaded{
    [](int) { return true; },
    // overloads can be templates
    [](auto) {
        return false;
    }
};

static_assert(std::visit(has_int, int_or_float{2}));
static_assert(!std::visit(has_int, int_or_float{2.0f}));

} // namespace ac
