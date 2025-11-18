// Copyright 2025 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/meta/is_template_base_of.hpp>

namespace ac {

template<typename DerivedOperation>
struct operation_base;

template<typename T>
struct is_operation : is_template_base_of<operation_base, T> {};

template<typename T>
concept Operation = is_operation<T>::value;

} // namespace ac
