// Copyright 2026 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <type_traits>

namespace ac {

/// Concept of an empty type with an additional requirement to be trivial.
///
/// Objects of such type can be efficiently created on the fly instead of
/// adding `[[no_unique_address]]` complexity for efficient storage.
/// Even with `[[no_unique_address]]`, storage can use extra memory in case
/// the same empty type is used multiple times.
///
/// Having all of default/copy/move constructors and destructor to be trivial,
/// that is no-op for an empty type, implies that creating an object on the fly
/// instead of storing it has the same observable behavior.
///
/// This requirement holds for the typical use cases for empty types,
/// such as function objects.
template<typename T>
concept EmptyTrivial = std::is_empty_v<T> && std::is_trivial_v<T>;

} // namespace ac
