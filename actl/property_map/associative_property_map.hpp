/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/property_map/property_map.hpp>
#include <type_traits>

namespace ac {

/**
 * Associative property map with available const [] operator.
 */
template <class AssociativeContainer,
          class Key   = const typename std::remove_reference_t<AssociativeContainer>::key_type,
          class Value = typename std::remove_reference_t<AssociativeContainer>::mapped_type>
class associative_property_map
    : public container_property_map<false, AssociativeContainer, Key, Value, Value&, Value> {
public:
    using container_property_map<false, AssociativeContainer, Key, Value, Value&,
                                 Value>::container_property_map;

    Value& operator[](Key& key) { return this->data_[key]; }

    Value operator[](Key& key) const {
        auto it = this->data_.find(key);
        return it == this->data_.end() ? Value{} : it->second;
    }
};

template <class Container>
inline auto make_associative_property_map(Container&& container) {
    return associative_property_map<remove_rvalue_reference_t<Container>>(
        std::forward<Container>(container));
}

}  // namespace ac
