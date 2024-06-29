// Copyright 2022 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

namespace ac {

constexpr bool is_int(int) noexcept {
    return true;
}

constexpr bool is_int(double x) {
    if (x < 0)
        throw;
    return false;
}

} // namespace ac
