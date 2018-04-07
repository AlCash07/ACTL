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
#include <actl/type_traits/is_iterator.hpp>
#include <array>

namespace ac {

namespace detail {

template <class Sequence, class Key, class S = std::remove_reference_t<Sequence>,
          class Value = typename S::value_type, class Ref = typename S::reference>
class sequence_pm_base : public container_property_map<Sequence, Key, Value, Ref, false> {
    using base_t = container_property_map<Sequence, Key, Value, Ref, false>;

public:
    static_assert(is_random_access_iterator<typename S::iterator>::value,
                  "sequence must be random access");

    using base_t::base_t;

    friend Ref get(sequence_pm_base& pm, Key key) {
        ACTL_ASSERT(0 <= key && key < this->data_.size());
        return pm.data_[key];
    }

    template <bool W = base_t::writeable>
    friend std::enable_if_t<W> put(sequence_pm_base& pm, Key key, Value value) {
        ACTL_ASSERT(0 <= key && key < this->data_.size());
        pm.data_[key] = value;
    }

    template <bool W = base_t::writeable>
    std::enable_if_t<W> clear() {
        fill(this->data_, Value{});
    }
};

template <class It, class V = std::pair<int, typename std::iterator_traits<It>::reference>>
class sequence_iterator : public iterator_facade<sequence_iterator<It, V>, std::input_iterator_tag,
                                                 V, V, const V*, void> {
public:
    sequence_iterator(It it, It begin) : it_{it}, begin_{begin} {}

private:
    friend struct ac::iterator_core_access;

    V dereference() const { return {static_cast<int>(it_ - begin_), *it_}; }

    void increment() { ++it_; }

    bool equals(const sequence_iterator& other) const { return it_ == other.it_; }

    It it_;
    It begin_;
};

}  // namespace detail

/**
 * Property map from integer domain that returns sequence element with the key index.
 * Sequence is a fixed size, random access range or container.
 */
template <class Sequence, class Key = int>
class sequence_property_map : public detail::sequence_pm_base<Sequence, Key> {
    using base_t = detail::sequence_pm_base<Sequence, Key>;

public:
    using iterator = detail::sequence_iterator<typename base_t::iterator>;

    using base_t::base_t;

    iterator begin() { return {base_t::begin(), base_t::begin()}; }
    iterator end()   { return {base_t::end(),   base_t::begin()}; }
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
