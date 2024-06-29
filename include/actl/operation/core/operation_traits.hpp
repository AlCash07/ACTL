// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/meta/nesting_depth.hpp>
#include <limits>

namespace ac {

struct operation_tag {};

template <class T>
concept Operation = requires { typename T::operation_category; };

template <class Operation, class T>
struct identity_element;

// TODO: we shouldn't need this.
template <Operation T>
struct nesting_depth<T> : constant<std::numeric_limits<size_t>::max()> {};

} // namespace ac
