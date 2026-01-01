// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/meta/concepts/comparison/EqualityComparable.hpp>
#include <actl/meta/concepts/object/SemiRegular1.hpp>

namespace ac {

template<typename T>
concept NothrowRegular = NothrowSemiRegular<T> && NothrowEqualityComparable<T>;

} // namespace ac
