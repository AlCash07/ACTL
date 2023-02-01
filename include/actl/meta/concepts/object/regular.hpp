// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

/// @file Extensions for the `std::regular` concept
/// https://en.cppreference.com/w/cpp/concepts/regular

#include <actl/meta/concepts/comparison/equality_comparable.hpp>
#include <actl/meta/concepts/object/semiregular.hpp>

namespace ac {

template <class T>
concept NothrowRegular = NothrowSemiregular<T> && NothrowEqualityComparable<T>;

} // namespace ac
