/***************************************************************************************************
 * Copyright 2019 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/string/ctype.hpp>
#include <cstdint>
#include <limits>

namespace ac::io::detail {

template <uint8_t MaxBase, class D, class UInt>
inline bool read_digit(D& id, UInt& x, uint8_t base) {
    if constexpr (MaxBase <= 10) {
        x = static_cast<UInt>(id.peek() - '0');
    } else {
        auto c = id.peek();
        if (is_digit(c)) {
            x = static_cast<UInt>(c - '0');
        } else if (is_upper(c)) {
            x = static_cast<UInt>(c - 'A' + 10);
        } else if (is_lower(c)) {
            x = static_cast<UInt>(c - 'a' + 10);
        } else {
            x = std::numeric_limits<UInt>::max();
        }
    }
    bool ok = x < base;
    if (ok) id.move(1);
    return ok;
}

}  // namespace ac::io::detail
