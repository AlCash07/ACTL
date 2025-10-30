// Copyright 2023 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <concepts>

namespace ac {

template<typename L, typename R>
concept TriviallyAssignableFrom =
    std::assignable_from<L, R> && std::is_trivially_assignable_v<L, R>;

template<typename L, typename R>
concept NothrowAssignableFrom =
    std::assignable_from<L, R> && std::is_nothrow_assignable_v<L, R>;

} // namespace ac
