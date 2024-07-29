// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/functional/category/detail/is_free_function.hpp>

namespace ac {

/// Concept of a free function.
template<class T>
concept FreeFunction = detail::is_free_function<T>::value;

} // namespace ac
