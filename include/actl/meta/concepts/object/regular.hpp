// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/meta/concepts/comparison/equality_comparable.hpp>
#include <actl/meta/concepts/object/semiregular.hpp>

namespace ac {

template<typename T>
concept NothrowRegular = NothrowSemiregular<T> && NothrowEqualityComparable<T>;

} // namespace ac
