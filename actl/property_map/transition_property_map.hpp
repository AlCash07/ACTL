/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/iterator/transition_iterator.hpp>
#include <actl/property_map/sequence_property_map.hpp>

namespace ac {

/**
 * Sequence property map with traversal interface that skips default values.
 */
template <class Sequence, class Key = int>
class transition_property_map : public detail::sequence_pm_base<Sequence, Key> {
    using base_t = detail::sequence_pm_base<Sequence, Key>;
    using base_t::data_;

public:
    using iterator = transition_iterator<typename base_t::iterator, Key>;

    using base_t::base_t;

    iterator begin() const { return {data_().begin(), data_().begin(), data_().end()}; }
    iterator end()   const { return {data_().end(), data_().begin(), data_().end()}; }
};

template <class Key = int, class Sequence>
inline auto make_transition_property_map(Sequence&& sequence) {
    return transition_property_map<remove_rvalue_reference_t<Sequence>, Key>(
        std::forward<Sequence>(sequence));
}

template <class T, int N, class Key = int>
using transition_array_property_map = transition_property_map<std::array<T, N>, Key>;

}  // namespace ac
