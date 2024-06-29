// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <type_traits>

namespace ac {

template<class T>
struct inout {
    struct enable_operators;

    T x;
};

template<class T>
inout(T&&) -> inout<T>;

template<class T>
struct is_inout : std::false_type {};

template<class T>
struct is_inout<inout<T>> : std::true_type {};

template<class T>
inline constexpr bool is_inout_v = is_inout<T>::value;

} // namespace ac
