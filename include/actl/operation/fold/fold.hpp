// Copyright 2021 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/operation/core/composite_operation.hpp>

namespace ac {

struct fold_t {
    static constexpr index inner_count = 1;
};

constexpr operation_composer<fold_t> fold;

}  // namespace ac
