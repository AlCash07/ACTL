// Copyright 2021 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/operation/operation/composite_operation.hpp>

namespace ac {

struct fold_ {
    static constexpr size_t inner_count = 1;
};

inline constexpr operation_composer<fold_> fold;

} // namespace ac
