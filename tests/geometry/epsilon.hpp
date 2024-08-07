// Copyright 2019 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/operation/policy/common/absolute_error.hpp>

struct epsilon9 {
    static constexpr double epsilon() {
        return 1e-9;
    }
};

ac::absolute_error<epsilon9> absolute_error9;
