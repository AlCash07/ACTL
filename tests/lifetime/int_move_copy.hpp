// Copyright 2026 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/lifetime/copy/AC_DISABLE_COPY.hpp>
#include <actl_test/core/copy_may_throw.hpp>
#include <actl_test/core/move_may_throw.hpp>

namespace ac {

/// move-only int wrapper
struct int_mo {
    int value = 0;

    constexpr int_mo(int x = 0) noexcept : value{x} {}
    int_mo(int_mo&&) = default;
    int_mo& operator=(int_mo&&) = default;
    AC_DISABLE_COPY(int_mo)
};

/// move-only int wrapper where move may throw
struct int_momt
    : int_mo
    , move_may_throw {
    using int_mo::int_mo;
};

/// int wrapper where copy may throw
struct int_cmt : copy_may_throw {
    int value = 0;

    constexpr int_cmt(int x = 0) noexcept : value{x} {}
};

// Wrapper where move may throw but copy may not
// is not provided, because it makes no practical sense.
//
// As an example of trivially copyable class
// `int` can be used directly without any wrappers.

} // namespace ac
