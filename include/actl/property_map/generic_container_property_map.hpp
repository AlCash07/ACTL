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
    using C   = remove_cvref_t<Container>;

public:
    using key_type   = container_id<C>;
    using value_type = typename C::value_type;
    using reference  = typename PMC::reference;

    static constexpr bool invertible = false;
    static constexpr bool iterable   = false;
    static constexpr bool writable   = PMC::writable;

    template <class... Ts>
    explicit generic_container_property_map(Ts&&... args) : data_{{std::forward<Ts>(args)...}} {}

    friend reference get(const generic_container_property_map& pm, key_type key) {
        return id_at(pm.data_(), key);
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
