/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/property_map/property_map.hpp>

namespace ac {

/**
 * Associative property map that returns default value for keys not present in container.
 */
template <class Container, class Key = typename std::remove_reference_t<Container>::key_type,
          class Value = typename std::remove_reference_t<Container>::mapped_type>
class associative_property_map
    : public container_property_map<Container, Key, Value, Value, false> {
    using base_t = container_property_map<Container, Key, Value, Value, false>;

public:
    using base_t::base_t;

    friend Value get(associative_property_map& pm, Key key) {
        auto it = pm.data_.find(key);
        return it == pm.data_.end() ? Value{} : it->second;
    }

    template <bool W = base_t::writeable>
    friend std::enable_if_t<W> put(associative_property_map& pm, Key key, Value value) {
        pm.data_[key] = value;
    }

    template <bool W = base_t::writeable>
    std::enable_if_t<W> clear() {
        this->data_.clear();
    }
};

template <class Container>
inline auto make_associative_property_map(Container&& container) {
    return associative_property_map<remove_rvalue_reference_t<Container>>(
        std::forward<Container>(container));
}

}  // namespace ac
