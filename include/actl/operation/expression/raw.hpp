// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <type_traits>

namespace ac {

template<typename T>
struct raw : std::remove_cvref<T> {};

template<typename T>
using raw_t = typename raw<T>::type;

} // namespace ac
