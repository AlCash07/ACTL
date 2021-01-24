// Copyright 2019 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/utility/index.hpp>

namespace ac::io::detail {

template <class T, class U>
constexpr index digit_count(T x, U base) {
    return x == 0 ? 0 : 1 + digit_count(x / base, base);
}

} // namespace ac::io::detail
