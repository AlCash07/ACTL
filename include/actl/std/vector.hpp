// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/io/argument/range.hpp>
#include <vector>

namespace ac {

template <class T, class A>
struct range_properties<std::vector<T, A>> : default_range_properties {
    static constexpr bool is_container = true;
};

} // namespace ac
