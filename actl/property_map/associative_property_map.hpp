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
template <class AssociativeContainer>
class associative_property_map : public property_map_base {
    using C   = std::remove_reference_t<AssociativeContainer>;
    using PMC = property_map_container<AssociativeContainer>;

public:
    using key_type   = typename C::key_type;
    using value_type = typename C::mapped_type;
    using reference  = value_type;
    using iterator   = typename PMC::iterator;

    static constexpr bool invertible = false;
    static constexpr bool iterable   = true;
    static constexpr bool writable   = PMC::writable;

    template <class... Ts>
    explicit associative_property_map(Ts&&... args) : data_{{std::forward<Ts>(args)...}} {}

    friend value_type get(const associative_property_map& pm, key_type key) {
        auto it = pm.data_().find(key);
        return it == pm.data_().end() ? value_type{} : it->second;
    }

    template <bool W = writable>
    friend std::enable_if_t<W> put(const associative_property_map& pm, key_type key,
                                   value_type value) {
        pm.data_()[key] = value;
    }

    iterator begin() const { return data_().begin(); }
    iterator end()   const { return data_().end(); }

    template <bool W = writable>
    std::enable_if_t<W> clear() {
        data_().clear();
    }

private:
    PMC data_;
};

template <class Container>
inline auto make_associative_property_map(Container&& container) {
    return associative_property_map<remove_rvalue_reference_t<Container>>(
        std::forward<Container>(container));
}

}  // namespace ac
