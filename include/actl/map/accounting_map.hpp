/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/map/traits.hpp>
#include <actl/std/vector.hpp>
#include <actl/util/compressed_pair.hpp>

namespace ac {

/**
 * Property map into integer domain that assigns next non-negative integer to every key not
 * encountered before. Can be invertible with overhead of additional vector of pointers.
 */
template <class AssociativeContainer, bool Invertible = false>
class accounting_map {
    using AC = AssociativeContainer;
    using K = typename AC::key_type;

public:
    struct is_accounting_map;

    using key_type = const K&;
    using value_type = typename AC::mapped_type;
    using reference = const value_type&;
    using range_type = const AC&;

    static constexpr bool invertible = Invertible;

    static_assert(is_unique_associative_container_v<AC> && is_pair_associative_container_v<AC>);
    static_assert(std::is_integral_v<value_type>, "value type must be integral");

    reference get(key_type key) {
        auto& ac = data_.first();
        auto pair = ac.insert({key, static_cast<value_type>(ac.size())});
        if constexpr (Invertible) {
            if (pair.second) data_.second().push_back(&pair.first->first);
        }
        return pair.first->second;
    }

    key_type invert(value_type value) const { return *data_.second()[static_cast<size_t>(value)]; }

    range_type map_range() const { return data_.first(); }

private:
    compressed_pair<AC, std::conditional_t<Invertible, std::vector<const K*>, none>> data_;
};

template <class AM>
struct map_traits<AM, std::void_t<typename AM::is_accounting_map>>
    : member_map_ops<AM, AM> {
    static constexpr bool readable = !std::is_const_v<AM>;
    static constexpr bool writable = false;
    static constexpr bool iterable = true;
};

}  // namespace ac
