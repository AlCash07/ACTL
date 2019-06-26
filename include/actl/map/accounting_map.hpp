/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/map/map.hpp>
#include <actl/std/vector.hpp>
#include <actl/util/compressed_pair.hpp>

namespace ac {

/**
 * Property map into integer domain that assigns next non-negative integer to every key not
 * encountered before. Can be invertible with overhead of additional vector of pointers.
 */
template <class AssociativeContainer, bool Invertible = false>
class accounting_map {
    using V = typename AssociativeContainer::mapped_type;

public:
    static_assert(is_unique_associative_container_v<AssociativeContainer> &&
                  is_pair_associative_container_v<AssociativeContainer>);

    using key_type = typename AssociativeContainer::key_type;
    using reference = const V&;

    static_assert(std::is_integral_v<V>, "value type must be integral");

    friend reference get(accounting_map& map, key_type key) {
        auto& ac = map.data_.first();
        auto pair = ac.insert({key, static_cast<V>(ac.size())});
        if constexpr (Invertible) {
            if (pair.second) map.data_.second().push_back(&pair.first->first);
        }
        return pair.first->second;
    }

    template <bool I = Invertible, class = std::enable_if_t<I>>
    friend key_type invert(const accounting_map& map, V value) {
        return *map.data_.second()[static_cast<size_t>(value)];
    }

    friend const AssociativeContainer& map_range(const accounting_map& map) {
        return map.data_.first();
    }

    void clear() {
        data_.first().clear();
        if constexpr (Invertible) {
            data_.second().clear();
        }
    }

private:
    using C = std::conditional_t<Invertible, std::vector<const key_type*>, none>;

    compressed_pair<AssociativeContainer, C> data_;
};

}  // namespace ac
