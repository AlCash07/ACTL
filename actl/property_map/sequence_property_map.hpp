/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/iterator/transition_iterator.hpp>
#include <actl/property_map/property_map.hpp>
#include <actl/range/algorithm.hpp>
#include <actl/type_traits/is_iterator.hpp>
#include <actl/type_traits/type_traits.hpp>
#include <array>

namespace ac {

/**
 * Property map from integer domain that returns range element with the key index.
 * Implements map traversal interface, skipping default values.
 */
template <class Range,
          class Key   = int,
          class Value = typename Range::value_type,
          class Ref   = typename Range::reference>
class range_property_map : public property_map<true, false, Key, Value, Ref, Ref> {
    static_assert(is_random_access_iterator<typename Range::iterator>::value,
                  "range must have random access");

public:
    using iterator       = transition_iterator<typename Range::iterator>;
    using const_iterator = iterator;

    explicit range_property_map(Range range) : data_{range} {}

    constexpr Ref operator [] (const Key& key) const { return data_[key]; }

    iterator begin() const { return {data_.begin(), data_.begin(), data_.end()}; }
    iterator end()   const { return {data_.end(),   data_.begin(), data_.end()}; }

private:
    Range data_;

    friend void clear(range_property_map& map) { fill(map.data_, Value{}); }
};

template <class Range>
inline auto make_range_property_map(Range range) { return range_property_map<Range>(range); }

template <class Key, class Range>
inline auto make_range_property_map(Range range) { return range_property_map<Range, Key>(range); }

/**
 * Range property map with underlying sequence container.
 */
template <class SequenceContainer, class Key = int,
          class Container = remove_reference_t<SequenceContainer>,
          class Value     = typename Container::value_type,
          class Ref       = typename Container::reference,
          class CRef      = typename Container::const_reference>
class sequence_property_map : public property_map<true, false, Key, Value, Ref, CRef> {
    static_assert(is_random_access_iterator<typename Container::iterator>::value,
                  "container must have random access");

public:
    using iterator       = transition_iterator<typename Container::iterator>;
    using const_iterator = transition_iterator<typename Container::const_iterator>;

    template <class... Ts>
    explicit constexpr sequence_property_map(Ts&&... args) : data_(std::forward<Ts>(args)...) {}

    constexpr Ref operator [] (const Key& key) { return data_[key]; }

    constexpr CRef operator [] (const Key& key) const { return data_[key]; }

    iterator begin() { return {data_.begin(), data_.begin(), data_.end()}; }
    iterator end()   { return {data_.end(),   data_.begin(), data_.end()}; }

    const_iterator begin() const { return {data_.begin(), data_.begin(), data_.end()}; }
    const_iterator end()   const { return {data_.end(),   data_.begin(), data_.end()}; }

private:
    SequenceContainer data_;

    friend void clear(sequence_property_map& map) { fill(map.data_, Value{}); }
};

template <class Container>
inline auto make_sequence_property_map(Container&& container) {
    return sequence_property_map<remove_rvalue_reference_t<Container>>(
        std::forward<Container>(container));
}

template <class Key, class Container>
inline auto make_sequence_property_map(Container&& container) {
    return sequence_property_map<remove_rvalue_reference_t<Container>, Key>(
        std::forward<Container>(container));
}

/**
 * Range property map with underlying fixed-size array.
 */
template <class T, int N, class Key = int>
using array_property_map = sequence_property_map<std::array<T, N>, Key>;

}  // namespace ac
