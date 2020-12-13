// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

namespace ac {

inline constexpr bool is_digit(char c) { return static_cast<unsigned char>(c - '0') < 10; }

inline constexpr bool is_lower(char c) { return static_cast<unsigned char>(c - 'a') < 26; }

inline constexpr bool is_upper(char c) { return static_cast<unsigned char>(c - 'A') < 26; }

inline constexpr bool is_space(char c) {
    return static_cast<unsigned char>(c - '\t') < 5 || c == ' ';
}

}  // namespace ac
