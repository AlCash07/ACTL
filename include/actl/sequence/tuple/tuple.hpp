// Copyright 2026 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/memory/empty_type/AC_EMPTY_BASES.hpp>
#include <actl/sequence/tuple/detail/tuple_element.hpp>
#include <utility> // for std::index_sequence

namespace ac {

namespace detail {

// It would be simpler to derive tuple_data directly from tuple_element.
// However, MSVC has a defect which results in the tuple size increase
// with such implementation even with `__declspec(empty_bases)`.
// This extra intermediate class is necessary as a workaround.
template<size_t Index, typename T>
struct AC_EMPTY_BASES tuple_element_derived : tuple_element<Index, T> {};

// std::index_sequence expansion is performed in this separate class
// instead of the tuple itself to keep the tuple type shorter.
template<typename Indices, typename... Elements>
struct tuple_data;

template<size_t... Is, typename... Elements>
struct AC_EMPTY_BASES tuple_data<std::index_sequence<Is...>, Elements...>
    : tuple_element_derived<Is, Elements>... {};

} // namespace detail

// Tuple is a sequence of elements that can have different types.
// Elements are stored contiguously like in a struct (save for padding),
// but they don't have names and can only be accessed via indices.
//
// This implementation is recommended to be used over
// `std::tuple <https://cppreference.com/cpp/utility/tuple>`_
// for the following reasons:
//
// 1. `ac::tuple` produces more efficient generated code,
// 	  because it's trivially copyable when the element types allow it.
//	  `std::tuple`, for comparison, is not even trivially move assignable.
// 2. `ac::tuple` completely avoids storage overhead for empty elements,
//	  unlike `std::tuple` on MSVC.
// 3. `ac::tuple` has no constructors and instead relies on
//	  aggregate initialization and separate conversion functions.
//    `std::tuple` in C++23 has 28 constructors
//	  which blow up the compilation time even when they aren't used.
// 4. Some implementations of `std::tuple` (for example, from Microsoft)
//    are very slow because of recursion.
template<typename... Elements>
struct tuple
    : detail::tuple_data<std::index_sequence_for<Elements...>, Elements...> {};

} // namespace ac
