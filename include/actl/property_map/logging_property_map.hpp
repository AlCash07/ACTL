/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/property_map/property_map.hpp>
#include <utility>

namespace ac {

/**
 * Property map that writes all key-value pairs from put operations into output iterator.
 */
template <class Map, class OutputIterator>
class logging_property_map : public property_map_wrapper_t<Map> {
    using base_t = property_map_wrapper_t<Map>;

    mutable OutputIterator it_;

public:
    template <class... Ts>
    explicit logging_property_map(OutputIterator it, Ts&&... args)
        : base_t(std::forward<Ts>(args)...), it_{it} {}

    friend auto get(const logging_property_map& pm, typename base_t::key_type key) {
        return get(static_cast<const base_t&>(pm), key);
    }

    friend void put(const logging_property_map& pm, typename base_t::key_type key,
        typename base_t::value_type value) {
        *pm.it_++ = std::make_pair(key, value);
        return put(static_cast<const base_t&>(pm), key, value);
    }
};

template <class Map, class OutputIterator, class... Ts>
inline auto make_logging_property_map(OutputIterator it, Ts&&... args) {
    return logging_property_map<Map, OutputIterator>(it, std::forward<Ts>(args)...);
}

}  // namespace ac
