// Copyright 2021 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/operation/scalar/enable_operators.hpp>
#include <actl/operation/scalar_operation.hpp>

namespace ac {

// clang-format off
struct comparison_operation : scalar_operation {};
struct equality_operation   : comparison_operation {};
struct ordering_operation   : comparison_operation {};
// clang-format on

template<typename T>
concept ComparisonOperation =
    ScalarOperation<T> &&
    std::derived_from<typename T::operation_category, comparison_operation>;

} // namespace ac
