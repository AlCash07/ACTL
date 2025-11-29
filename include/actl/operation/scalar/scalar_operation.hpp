// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/operation/operation/operation_base.hpp>
#include <actl/operation/scalar/enable_operators.hpp>

namespace ac {

// clang-format off
struct scalar_operation_tag {};

struct bit_operation_tag     : scalar_operation_tag {};
struct bitwise_operation_tag : bit_operation_tag {};

struct comparison_operation_tag : scalar_operation_tag {};
struct equality_operation_tag   : comparison_operation_tag {};
struct ordering_operation_tag   : comparison_operation_tag {};

struct logical_operation_tag : scalar_operation_tag {};
// clang-format on

#if 0
template<typename T>
concept ComparisonOperation =
    ScalarOperation<T> &&
    std::derived_from<typename T::operation_category, comparison_operation_tag>;
#endif

} // namespace ac
