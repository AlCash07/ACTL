// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/map/traits.hpp>
#include <actl/memory/AC_NO_UNIQUE_ADDRESS.hpp>

namespace ac {

/// Map that writes all key-value pairs from put operations into output
/// iterator.
template<typename Map, typename OutIter>
struct logging_map {
    AC_NO_UNIQUE_ADDRESS Map map;
    AC_NO_UNIQUE_ADDRESS OutIter iter;

    operator Map&() {
        return map;
    }
};

template<typename M, typename OI>
logging_map(M&&, OI) -> logging_map<M, OI>;

template<typename M, typename OI>
struct map_traits<logging_map<M, OI>> : map_traits<M> {};

template<typename M, typename OI>
struct map_ops<logging_map<M, OI>> : map_ops<M> {
    static void put(
        logging_map<M, OI>& map, map_key_t<M> key, map_value_t<M> value
    ) {
        *map.iter++ = std::pair{key, value};
        ac::put(map.map, key, value);
    }
};

} // namespace ac
