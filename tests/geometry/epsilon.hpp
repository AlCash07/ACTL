/***************************************************************************************************
 * Copyright 2019 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/functional/comparison.hpp>

struct epsilon9 {
    static constexpr double epsilon() { return 1e-9; }
};

inline ac::op::absolute_error<epsilon9> absolute_error9;
