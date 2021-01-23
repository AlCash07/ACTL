// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/string/ctype.hpp>
#include <limits>

namespace ac::io {

enum digit_kind { digit, alnum };

template <class UInt, digit_kind kind>
UInt to_digit(char c) {
    static_assert(std::is_unsigned_v<UInt>);
    if constexpr (kind == digit) {
        return static_cast<UInt>(c - '0');
    } else {
        if (ac::is_digit(c))
            return static_cast<UInt>(c - '0');
        else if (is_upper(c))
            return static_cast<UInt>(c - 'A' + 10);
        else if (is_lower(c))
            return static_cast<UInt>(c - 'a' + 10);
        else
            return std::numeric_limits<UInt>::max();
    }
}

template <class UInt>
bool is_digit(UInt digit, UInt base) {
    return digit < base;
}

}  // namespace ac::io
