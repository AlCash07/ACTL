/***************************************************************************************************
 * Copyright 2019 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/functional/composite/tuple.hpp>
#include <actl/io/tuple.hpp>
#include <actl/numeric/hash.hpp>
#include <utility>

namespace ac {

template <class T1, class T2>
constexpr size_t hash_value(const std::pair<T1, T2>& x) {
    return hash_value(x.first, x.second);
}

}  // namespace ac
