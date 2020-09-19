/***************************************************************************************************
 * Copyright 2019 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/functional/scalar/all.hpp>

struct epsilon9 {
    static constexpr double epsilon() { return 1e-9; }
};

inline ac::math::absolute_error<epsilon9> absolute_error9;
