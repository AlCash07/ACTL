// Standard containers traits and categorization.
//
// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/container/dummy_container.hpp>
#include <actl/meta/rebind.hpp>

namespace ac {

template<typename C, typename Target>
using rebind_container_t = std::conditional_t<
    std::is_same_v<C, none> || std::is_same_v<Target, none>,
    dummy_container,
    rebind_t<C, Target>>;

} // namespace ac
