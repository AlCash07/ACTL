// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

namespace ac {

/// Checks if @p c is a decimal digit.
inline constexpr bool is_digit(char c) noexcept
{
    return static_cast<unsigned char>(c - '0') < 10;
}

/// Checks if @p c is a lowercase letter.
inline constexpr bool is_lower(char c) noexcept
{
    return static_cast<unsigned char>(c - 'a') < 26;
}

/// Checks if @p c is an uppercase letter.
inline constexpr bool is_upper(char c) noexcept
{
    return static_cast<unsigned char>(c - 'A') < 26;
}

/// Checks if @p c is an alphabetic character (a letter).
inline constexpr bool is_alpha(char c) noexcept
{
    return is_upper(c & ~0x20);
}

/// Checks if @p c is an alphabetic character or a decimal digit.
inline constexpr bool is_alphanumeric(char c) noexcept
{
    return is_alpha(c) || is_digit(c);
}

/// Checks if @p c is a whitespace character, that is one of:
/// - space (0x20, ' ')
/// - form feed (0x0c, '\f')
/// - line feed (0x0a, '\\n')
/// - carriage return (0x0d, '\r')
/// - horizontal tab (0x09, '\t')
/// - vertical tab (0x0b, '\v')
inline constexpr bool is_space(char c) noexcept
{
    return static_cast<unsigned char>(c - '\t') < 5 || c == ' ';
}

} // namespace ac
