/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/map/traits.hpp>
#include <actl/util/compressed_pair.hpp>

namespace ac {

/**
 * Map that writes all key-value pairs from put operations into output iterator.
 */
template <class Map, class OutputIterator>
class logging_map : public compressed_pair<Map, OutputIterator> {
public:
    using compressed_pair<Map, OutputIterator>::compressed_pair;

    operator Map&() { return this->first(); }
};

template <class M, class OI>
logging_map(M&&, OI) -> logging_map<M, OI>;

template <class M, class OI>
struct map_traits<logging_map<M, OI>> : map_traits<M> {};

template <class M, class OI>
struct map_ops<logging_map<M, OI>> : map_ops<M> {
    static void put(logging_map<M, OI>& map, map_key_t<M> key, map_value_t<M> value) {
        *map.second()++ = std::pair{key, value};
        ac::put(map.first(), key, value);
    }
};

}  // namespace ac
