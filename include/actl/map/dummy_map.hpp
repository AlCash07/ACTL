// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/map/traits.hpp>

namespace ac {

class dummy_map {};

template<>
struct const_map_traits<dummy_map>
    : map_traits_base<void, void, void, false, true> {};

template<typename K, typename V>
constexpr void put(dummy_map, K, V) {}

} // namespace ac
