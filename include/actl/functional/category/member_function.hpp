// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/functional/category/detail/as_free_function.hpp>

namespace ac {

template<class T>
concept MemberFunction = detail::as_free_function<T>::is_member_function;

} // namespace ac
