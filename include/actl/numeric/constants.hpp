// Copyright 2019 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

namespace ac {

template <class T = double>
constexpr T pi_v = static_cast<T>(3.141592653589793238462643383279502884L);

constexpr double pi = pi_v<>;

} // namespace ac
