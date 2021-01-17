// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/meta/is_template_base_of.hpp>
#include <actl/operation/scalar/scalar_operation.hpp>

namespace ac {

template <class Derived>
struct comparison_operation : scalar_operation<Derived, 2, scalar_tag> {};

template <class T>
constexpr bool is_comparison_v = is_template_base_of_v<comparison_operation, T>;

}  // namespace ac
