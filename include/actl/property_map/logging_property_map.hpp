/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/property_map/property_map.hpp>
#include <actl/std/utility.hpp>

namespace ac {

/**
 * Property map that writes all key-value pairs from put operations into output iterator.
 */
template <class PM, class OutputIterator>
class logging_property_map : public property_map_wrapper_t<PM> {
    using base_t = property_map_wrapper_t<PM>;

    mutable OutputIterator it_;

public:
    explicit logging_property_map(PM&& pm, OutputIterator it) : base_t{std::move(pm)}, it_{it} {}

    friend void put(const logging_property_map& pm, typename base_t::key_type key,
        typename base_t::value_type value) {
        *pm.it_++ = std::pair{key, value};
        put(static_cast<const base_t&>(pm), key, value);
    }
};

}  // namespace ac
