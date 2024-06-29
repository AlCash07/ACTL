// Copyright 2022 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <type_traits>

namespace ac {

template<class T, class... Args>
inline constexpr bool can_list_initialize_v =
    requires(T, Args&&... args) { T{args...}; };

} // namespace ac
