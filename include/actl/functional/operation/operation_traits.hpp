// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/functional/operation/major_category.hpp>
#include <limits>

namespace ac {

// Base class for operations.
template <class Derived>
struct operation;

struct operation_tag {};

template <class T>
constexpr bool is_operation_v = std::is_same_v<operation_tag, category_t<T>>;

namespace detail {

template <class T>
struct type_depth<T, std::enable_if_t<is_operation_v<T>>>
    : index_constant<std::numeric_limits<index>::max()> {};

}  // namespace detail

/* Operation is_associative trait: defined by nested `struct is_associative;`. */

template <class T, class = void>
struct is_associative : std::false_type {};

template <class T>
struct is_associative<T, std::void_t<typename T::is_associative>> : std::true_type {};

template <class T>
constexpr bool is_associative_v = is_associative<T>::value;

/* Operation is_commutative trait: defined by nested `struct is_commutative;`. */

template <class T, class = void>
struct is_commutative : std::false_type {};

template <class T>
struct is_commutative<T, std::void_t<typename T::is_commutative>> : std::true_type {};

template <class T>
constexpr bool is_commutative_v = is_commutative<T>::value;

}  // namespace ac
