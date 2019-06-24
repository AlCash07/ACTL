/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/map/property_map.hpp>
#include <actl/std/utility.hpp>

namespace ac {

/**
 * Map that writes all key-value pairs from put operations into output iterator.
 */
template <class Map, class OutputIterator>
class logging_map : public map_wrapper_t<Map> {
    using base_t = map_wrapper_t<Map>;

    mutable OutputIterator it_;

public:
    explicit logging_map(Map&& map, OutputIterator it) : base_t{std::move(map)}, it_{it} {}

    friend void put(const logging_map& map, typename base_t::key_type key,
                    typename base_t::value_type value) {
        *map.it_++ = std::pair{key, value};
        put(static_cast<const base_t&>(map), key, value);
    }
};

}  // namespace ac
