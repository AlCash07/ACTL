// Copyright 2026 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/numeric/trigonometry/pi.hpp>

static_assert(std::is_constructible_v<float, ac::Pi>);
static_assert(std::is_constructible_v<double, ac::Pi>);

static_assert(!std::is_convertible_v<ac::Pi, float>);

static_assert(!std::is_constructible_v<int, ac::Pi>);

static_assert(static_cast<int>(float{ac::pi}) == 3);
