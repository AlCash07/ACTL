/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/assert.hpp>
#include <actl/iterator/iterator_facade.hpp>
#include <actl/property_map/property_map.hpp>
#include <actl/range/algorithm.hpp>
#include <actl/traits/is_iterator.hpp>
#include <array>

namespace ac {

namespace detail {

template <class Sequence, class Key>
class sequence_pm_base : public property_map_base {
    using PMC = property_map_container<Sequence>;

public:
    static_assert(is_random_access_iterator<typename PMC::iterator>::value,
                  "sequence must be random access");

    using key_type   = Key;
    using value_type = typename std::remove_reference_t<Sequence>::value_type;
    using reference  = typename PMC::reference;
    using iterator   = typename PMC::iterator;

    static constexpr bool invertible = false;
    static constexpr bool iterable   = true;
    static constexpr bool writable   = PMC::writable;

    template <class... Ts>
    explicit sequence_pm_base(Ts&&... args) : data_{{std::forward<Ts>(args)...}} {}

    friend reference get(const sequence_pm_base& pm, Key key) {
        ACTL_ASSERT(0 <= key && key < data_().size());
        return pm.data_()[key];
    }

    template <bool W = writable>
    std::enable_if_t<W> clear() {
        fill(data_(), value_type{});
    }

protected:
    PMC data_;
};

}  // namespace detail

/**
 * Property map from integer domain that returns sequence element with the key index.
 * Sequence is a fixed size, random access range or container.
 */
template <class Sequence, class Key = int>
class sequence_property_map : public detail::sequence_pm_base<Sequence, Key> {
    using base_t = detail::sequence_pm_base<Sequence, Key>;
    using It     = typename base_t::iterator;
    using Pair   = std::pair<Key, typename base_t::reference>;
    using base_t::data_;

public:
    class iterator : public iterator_adaptor<iterator, It, use_default, Pair, Pair, const Pair*> {
        iterator(It it, It begin)
            : iterator_adaptor<iterator, It, use_default, Pair, Pair, const Pair*>(it)
            , begin_{begin} {}

        Pair dereference() const {
            return {static_cast<Key>(this->base() - begin_), *this->base()};
        }

        It begin_;

        friend class sequence_property_map;
        friend struct ac::iterator_core_access;
    };

    using base_t::base_t;

    iterator begin() const { return {data_().begin(), data_().begin()}; }
    iterator end() const { return {data_().end(), data_().begin()}; }
};

template <class Key = int, class Sequence>
inline auto make_sequence_property_map(Sequence&& sequence) {
    return sequence_property_map<remove_rvalue_reference_t<Sequence>, Key>(
        std::forward<Sequence>(sequence));
}

/**
 * Range property map with underlying fixed-size array.
 */
template <class T, int N, class Key = int>
using array_property_map = sequence_property_map<std::array<T, N>, Key>;

}  // namespace ac
