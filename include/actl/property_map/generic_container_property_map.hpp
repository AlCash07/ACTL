/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/container/container_id.hpp>
#include <actl/property_map/property_map.hpp>

namespace ac {

template <class Container>
class generic_container_property_map : public property_map_base {
    using PMC = property_map_container<Container>;

public:
    using key_type   = typename Container::id;
    using value_type = typename Container::value_type;
    using reference  = typename PMC::reference;

    static constexpr bool invertible = false;
    static constexpr bool iterable   = false;
    static constexpr bool writable   = PMC::writable;

    template <class... Ts>
    explicit generic_container_property_map(Ts&&... args) : data_{{std::forward<Ts>(args)...}} {}

    friend reference get(const generic_container_property_map& pm, key_type key) {
        if constexpr (is_random_access_container_v<Container>) {
            return pm.data_()[key];
        } else {
            // const_cast is required because id contains a const_iterator.
            return const_cast<reference>(*key.get_iterator());
        }
    }

private:
    PMC data_;
};

template <class Container>
inline auto make_generic_container_property_map(Container&& container) {
    return generic_container_property_map<remove_rvalue_reference_t<Container>>(
        std::forward<Container>(container));
}

}  // namespace ac
