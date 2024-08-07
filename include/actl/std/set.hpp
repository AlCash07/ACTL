// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/range/traits/properties.hpp>
#include <set>

namespace ac {

template<class T, class C, class A>
struct range_properties<std::set<T, C, A>> : default_range_properties {
    static constexpr bool is_container = true;
    static constexpr bool is_sorted = true;
    static constexpr bool is_unique = true;
};

template<class T, class C, class A>
struct range_properties<std::multiset<T, C, A>> : default_range_properties {
    static constexpr bool is_container = true;
    static constexpr bool is_sorted = true;
};

} // namespace ac
