// Copyright 2023 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/meta/nesting_depth.hpp>
#include <actl/meta/tuple.hpp>
#include <actl/range/traits/associated_types.hpp>
#include <actl/range/traits/strict_range.hpp>

namespace ac {

template<Range T>
struct range_nesting_depth
    : size_constant<1 + nesting_depth_v<range_value_t<T>>> {};

template<Range T>
struct nesting_depth<T> : range_nesting_depth<T> {};

// Some types model both Range and Tuple, so we need to explicitly address this
// ambiguity. We do this by relying on the Range deduction, because it examines
// a single `range_value_t` type, so it's easier for a compiler.
// The type is considered invalid if examining it as a Tuple changes the depth.
template<class T>
    requires(Range<T> && Tuple<T>)
struct nesting_depth<T> : range_nesting_depth<T> {};

} // namespace ac
