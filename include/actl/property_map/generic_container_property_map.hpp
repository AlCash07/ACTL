/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/container/container_id.hpp>
#include <actl/property_map/property_map.hpp>
#include <actl/range/container_range.hpp>

namespace ac {

template <class Container>
class generic_container_property_map : public property_map_base, public container_range<Container> {
    using base_t = container_range<Container>;
    using C      = remove_cvref_t<Container>;

public:
    using key_type  = container_id<C>;
    using reference = std::conditional_t<detail::cr_traits<Container>::is_const,
                                         typename C::const_reference, typename C::reference>;

    static constexpr bool invertible = false;
    // TODO: make this iterable.
    static constexpr bool iterable   = false;
    static constexpr bool writable   = true;

    using base_t::base_t;

    friend reference get(const generic_container_property_map& pm, key_type key) {
        return id_at(pm.data_, key);
    }
};

template <class Container>
inline auto make_generic_container_property_map(Container&& container) {
    return generic_container_property_map<remove_rvalue_ref_t<Container>>(
        std::forward<Container>(container));
}

}  // namespace ac
