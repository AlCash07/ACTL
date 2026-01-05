// Copyright 2026 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#pragma once

namespace ac {

template<typename T>
struct value_if_cheap {
    using type = T;
};

template<typename T>
struct value_if_cheap<T&> {
    static constexpr bool is_copy_cheap = //
        sizeof(T) <= 2 * sizeof(void*) && std::is_trivially_copyable_v<T> &&
        std::is_copy_constructible_v<T>;
    // is_copy_constructible_v is necessary because is_trivially_copyable_v
    // doesn't imply it, for example, for C arrays like `char[2]`

    using type = std::conditional_t<is_copy_cheap, T, T const&>;
};

template<typename T>
struct value_if_cheap<T const&> : value_if_cheap<T&> {};

template<typename T>
using value_if_cheap_t = typename value_if_cheap<T>::type;

} // namespace ac
