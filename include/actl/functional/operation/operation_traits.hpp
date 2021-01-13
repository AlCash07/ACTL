// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/category/category.hpp>
#include <actl/category/utility/nesting_depth.hpp>
#include <actl/traits/type_traits.hpp>
#include <limits>

namespace ac {

// Base class for operations.
template <class Derived>
struct operation;

struct operation_tag {
    using base = unclassified_tag;
};

template <class T>
constexpr bool is_operation_v = std::is_same_v<operation_tag, category_t<T>>;

template <class Operation, class T>
struct neutral_element;

template <>
struct nesting_depth<operation_tag> : index_constant<std::numeric_limits<index>::max()> {};

}  // namespace ac
