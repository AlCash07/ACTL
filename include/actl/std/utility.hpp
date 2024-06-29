// Copyright 2019 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/numeric/hash.hpp>
#include <actl/operation/tuple/comparison.hpp>
#include <utility>

namespace ac {

template <class T1, class T2>
constexpr size_t hash_value(std::pair<T1, T2> const& x) {
    return hash_value(x.first, x.second);
}

} // namespace ac
