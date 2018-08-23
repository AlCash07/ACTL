/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/container/generic_container.hpp>
#include <actl/property_map/property_map.hpp>

namespace ac {

template <class GenericContainer>
class generic_container_property_map : public property_map_base {
    using PMC = property_map_container<GenericContainer>;

public:
    using key_type   = typename GenericContainer::id;
    using value_type = typename GenericContainer::value_type;
    using reference  = typename PMC::reference;

    static constexpr bool invertible = false;
    static constexpr bool iterable   = false;
    static constexpr bool writable   = PMC::writable;

    generic_container_property_map(GenericContainer& container) : data_{container} {}

    friend reference get(const generic_container_property_map& pm, key_type key) {
        return pm.data_[key];
    }

private:
    GenericContainer& data_;
};

template <class GenericContainer>
inline auto make_generic_container_property_map(GenericContainer& container) {
    return generic_container_property_map<GenericContainer>(container);
}

}  // namespace ac
