// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/category/category.hpp>
#include <actl/category/utility/is_subcategory_of.hpp>
#include <actl/category/utility/nesting_depth.hpp>
#include <limits>

namespace ac {

struct operation_tag
{
    using base = unclassified_tag;
};

template <class T>
inline constexpr bool is_operation_v =
    is_subcategory_of_v<category_t<T>, operation_tag>;

template <class Operation, class T>
struct identity_element;

template <class T>
struct nesting_depth<T, std::enable_if_t<is_operation_v<T>>>
    : index_constant<std::numeric_limits<index>::max()>
{};

} // namespace ac
