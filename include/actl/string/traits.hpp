// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <string_view>

namespace ac {

template<typename T, typename Char = char>
inline constexpr bool is_string_v =
    std::is_convertible_v<T, std::basic_string_view<Char>>;

} // namespace ac
