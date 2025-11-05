// Copyright 2025 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <type_traits>

namespace ac {

template<typename T>
struct is_reference : std::false_type {};

/// Reference is an opt-in concept for types that
/// don't own data and only reference it, for example:
/// - built-in reference `T&`;
/// - `std::span<T>`, which references a contiguous array of `T`;
/// - single bit reference like the one used in a bitset.
///
/// Types can opt in by specializing ac::is_reference struct
/// to contain `static constexpr bool value = true;`, for example
/// @code
/// template<typename T>
/// struct is_reference<T&> : std::true_type {};
/// @endcode
/// If a type isn't correctly recognized as a Reference,
/// then the header with this specialization should be included, for example
/// @code
/// #include <actl/std/span.hpp>
/// @endcode
///
/// @note Pointer isn't considered to be a Reference
/// and it's not recommended to use raw pointers overall,
/// because there are less error-prone alternatives
/// for the two most popular use cases for pointers:
/// 1. To reference a single optional value,
///    `std::optional<T&>` is better, because it disallows
///    pointer arithmetic that makes no sense in such case.
/// 2. To reference a contiguous array,
///    `std::span<T>` is better, because it keeps information
///    about the array size, which prevents accidental use of
///    incorrect size and enables checks for out-of-bounds access.
template<typename T>
concept Reference = is_reference<T>::value;

template<typename T>
struct is_reference<T&> : std::true_type {};
template<typename T>
struct is_reference<T&&> : std::true_type {};

} // namespace ac
