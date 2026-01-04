// Copyright 2026 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#pragma once

namespace ac {

/// copy_may_throw should be used as a base class to define
/// copy constructor and assignment operator with `noexcept(false)`,
/// which also ensures that copy is never trivial.
/// For example,
/// @code
/// struct my_int : ac::copy_may_throw {
///     int value = 0;
/// };
/// @endcode
///
/// This is mostly useful in testing containers' behavior
/// for different element types.
struct copy_may_throw {
    constexpr copy_may_throw(const copy_may_throw&) noexcept(false) {}
    constexpr copy_may_throw& operator=(const copy_may_throw&) noexcept(false) {
        return *this;
    }

    copy_may_throw(copy_may_throw&&) = default;
    copy_may_throw& operator=(copy_may_throw&&) = default;

    // Default constructor is defined because it would be deleted otherwise.
    copy_may_throw() = default;
};

} // namespace ac
