/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/graph/events.hpp>
#include <actl/property_map/property_map.hpp>

namespace ac {

template <auto Ptr>
struct vertex_initializer;

template <class Base, class Member, Member Base::*Ptr>
struct vertex_initializer<Ptr> : Base {
    using vertex = typename property_traits<Member>::key_type;

    typename property_traits<Member>::value_type value;

    using Base::operator();

    void operator()(on_vertex_initialize, vertex u) const { put(this->*Ptr, u, value); }

    bool operator()(is_vertex_discovered, vertex u) const { return get(this->*Ptr, u) != value; }
};

}  // namespace ac
