/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/map/property_map.hpp>
#include <actl/range/container_range.hpp>
#include <actl/traits/type_traits.hpp>

namespace ac {

/**
 * Associative property map that returns default value for keys not present in container.
 */
template <class AssociativeContainer>
class associative_map : property_map_base, public container_range<AssociativeContainer> {
    using base_t = container_range<AssociativeContainer>;
    using C = remove_cvref_t<AssociativeContainer>;

public:
    using key_type = typename C::key_type;
    using value_type = typename C::mapped_type;
    using reference = value_type;

    static constexpr bool invertible = false;
    static constexpr bool iterable = true;

    using base_t::base_t;

    friend value_type get(const associative_map& pm, key_type key) {
        auto it = pm.data_.find(key);
        return it == pm.data_.end() ? value_type{} : it->second;
    }

    template <bool W = base_t::writable, std::enable_if_t<W, int> = 0>
    friend void put(const associative_map& pm, key_type key, value_type value) {
        pm.data_[key] = value;
    }

    template <bool W = base_t::writable, class = std::enable_if_t<W>>
    void clear() {
        this->data_.clear();
    }
};

template <class AC>
associative_map(AC&&) -> associative_map<remove_rvalue_ref_t<AC>>;

}  // namespace ac
