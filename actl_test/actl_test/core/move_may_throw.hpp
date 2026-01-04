// Copyright 2026 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#pragma once

namespace ac {

/// move_may_throw should be used as a base class to define
/// move constructor and assignment operator with `noexcept(false)`,
/// which also ensures that move is never trivial.
/// For example,
/// @code
/// struct my_int : ac::move_may_throw {
///     int value = 0;
/// };
/// @endcode
///
/// This is mostly useful in testing containers' behavior
/// for different element types.
///
/// @note Outside of testing, move should mostly be `noexcept(true)`.
struct move_may_throw {
    constexpr move_may_throw(move_may_throw&&) noexcept(false) {}
    constexpr move_may_throw& operator=(move_may_throw&&) noexcept(false) {
        return *this;
    }

    move_may_throw(const move_may_throw&) = default;
    move_may_throw& operator=(const move_may_throw&) = default;

    // Default constructor is defined because it would be deleted otherwise.
    move_may_throw() = default;
};

} // namespace ac
