/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/graph/events.hpp>
#include <actl/map/property_map.hpp>

namespace ac {

template <class Base>
struct vertex_initializer : Base {
    typename Base::value_type value;

    using Base::operator();

    void operator()(on_vertex_initialize, typename Base::key_type u) { put(*this, u, value); }
    bool operator()(is_vertex_discovered, typename Base::key_type u) {
        return get(*this, u) != value;
    }
};

}  // namespace ac
