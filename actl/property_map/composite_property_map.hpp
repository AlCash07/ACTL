/***************************************************************************************************
 * composite_property_map combines multiple property maps, applying them in the given order.
 * Supports inversion and traversal if at all possible.
 ***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <type_traits>
#include <utility>
#include <actl/iterator/iterator_adaptor.hpp>
#include <actl/property_map/property_map.hpp>
#include <actl/type/compressed_pair.hpp>

namespace ac {

namespace detail {

// Base class containing the maps and implementing common methods.
template <class OM, class IM, bool T, bool I,
          class Key   = typename property_traits<OM>::key_type,
          class Value = typename property_traits<IM>::value_type,
          class Ref   = typename property_traits<IM>::reference>
class composite_pm_base : public property_map<T, I, Key, Value, Ref> {
    static_assert(std::is_convertible<typename property_traits<OM>::reference,
                                      const typename property_traits<IM>::key_type&>::value,
                  "incompatible maps");

public:
    template <class... Ts>
    explicit constexpr composite_pm_base(Ts&&... args) : maps_(std::forward<Ts>(args)...) {}

    constexpr Ref operator [] (const Key& key) { return maps_.second()[maps_.first()[key]]; }

    constexpr const Ref operator [] (const Key& key) const {
        return maps_.second()[maps_.first()[key]];
    }

protected:
    compressed_pair<OM, IM> maps_;

    friend void clear(composite_pm_base& map) {
        clear(map.maps_.first());
        clear(map.maps_.second());
    }
};

// Class that adds invert method if possible.
template <class OM, class IM, bool T, bool, bool>
class composite_pm_invert : public composite_pm_base<OM, IM, T, false> {
public:
    using composite_pm_base<OM, IM, T, false>::composite_pm_base;
};

template <class OM, class IM, bool T>
class composite_pm_invert<OM, IM, T, true, true> : public composite_pm_base<OM, IM, T, true> {
    using base_type = composite_pm_base<OM, IM, T, true>;

public:
    using base_type::base_type;

    constexpr const typename base_type::key_type invert(
        const typename base_type::value_type& value) const {
        return this->maps_.first().invert(this->maps_.second().invert(value));
    }
};

// Middle class to simplify inheritance declarations.
template <class OM, class IM, bool T, bool I>
class composite_pm_mid : public composite_pm_invert<OM, IM, T, I, property_traits<IM>::invertible> {
public:
    using composite_pm_invert<OM, IM, T, I, property_traits<IM>::invertible>::composite_pm_invert;
};

// Class that adds begin/end methods if possible.
template <class OM, class IM, bool, bool, bool I>
class composite_pm_impl : public composite_pm_mid<OM, IM, false, I> {
public:
    using composite_pm_mid<OM, IM, false, I>::composite_pm_mid;
};

template <class Iterator, class IM, class Key, class Ref, class Pair = std::pair<Key, Ref>>
class pm_iterator : public iterator_adaptor<pm_iterator<Iterator, IM, Key, Ref, Pair>, Iterator,
                                            use_default, Pair, Pair, Pair*, use_default> {
public:
    pm_iterator(const Iterator& it, IM& map)
        : iterator_adaptor<pm_iterator<Iterator, IM, Key, Ref, Pair>, Iterator, use_default, Pair,
                           Pair, Pair*, use_default>(it),
          map_{map} {}

private:
    friend struct ac::iterator_core_access;

    Pair dereference() const { return {this->base()->first, map_[this->base()->second]}; }

    IM& map_;
};

template <class OM, class IM, bool T, bool I>
class composite_pm_impl<OM, IM, true, T, I> : public composite_pm_mid<OM, IM, true, I> {
    using base_type = composite_pm_mid<OM, IM, true, I>;

public:
    using iterator = pm_iterator<typename OM::iterator,
                                 IM,
                                 typename OM::key_type,
                                 typename property_traits<IM>::reference>;

    using const_iterator = pm_iterator<typename OM::const_iterator,
                                       const IM,
                                       typename OM::key_type,
                                       typename property_traits<IM>::const_reference>;

    using base_type::base_type;

    iterator begin() { return {this->maps_.first().begin(), this->maps_.second()}; }
    iterator end()   { return {this->maps_.first().end(),   this->maps_.second()}; }

    const_iterator begin() const { return {this->maps_.first().begin(), this->maps_.second()}; }
    const_iterator end()   const { return {this->maps_.first().end(),   this->maps_.second()}; }
};

template <class Iterator, class OM, class Key, class Ref, class Pair = std::pair<Key, Ref>>
class pm_invert_iterator
    : public iterator_adaptor<pm_invert_iterator<Iterator, OM, Key, Ref, Pair>, Iterator,
                              use_default, Pair, Pair, Pair*, use_default> {
public:
    pm_invert_iterator(const Iterator& it, OM& map)
        : iterator_adaptor<pm_invert_iterator<Iterator, OM, Key, Ref, Pair>, Iterator, use_default,
                           Pair, Pair, Pair*, use_default>(it),
          map_{map} {}

private:
    friend struct ac::iterator_core_access;

    Pair dereference() const { return {map_.invert(this->base()->first), this->base()->second}; }

    OM& map_;
};

template <class OM, class IM>
class composite_pm_impl<OM, IM, false, true, true> : public composite_pm_mid<OM, IM, true, true> {
    using base_type = composite_pm_mid<OM, IM, true, true>;

public:
    using iterator = pm_invert_iterator<typename IM::iterator,
                                        OM,
                                        typename property_traits<OM>::key_type,
                                        typename IM::reference>;

    using const_iterator = pm_invert_iterator<typename IM::const_iterator,
                                              const OM,
                                              typename property_traits<OM>::key_type,
                                              typename IM::const_reference>;

    using base_type::base_type;

    iterator begin() { return {this->maps_.second().begin(), this->maps_.first()}; }
    iterator end()   { return {this->maps_.second().end(),   this->maps_.first()}; }

    const_iterator begin() const { return {this->maps_.second().begin(), this->maps_.first()}; }
    const_iterator end()   const { return {this->maps_.second().end(),   this->maps_.first()}; }
};

// Type alias to simplify inheritance declarations.
template <class OM, class IM>
using composite_pm_impl_t =
    composite_pm_impl<OM, IM, property_traits<OM>::traversible, property_traits<IM>::traversible,
                      property_traits<OM>::invertible>;

}  // namespace detail

template <class OuterMap, class InnerMap, class... OtherMaps>
class composite_property_map
    : public composite_property_map<composite_property_map<OuterMap, InnerMap>, OtherMaps...> {};

template <class OuterMap, class InnerMap>
class composite_property_map<OuterMap, InnerMap>
    : public detail::composite_pm_impl_t<OuterMap, InnerMap> {
    using base_type = detail::composite_pm_impl_t<OuterMap, InnerMap>;

public:
    using base_type::base_type;
};

template <class Map>
inline Map make_composite_property_map(Map&& map) { return map; }

template <class OuterMap, class InnerMap, class... OtherMaps>
inline auto make_composite_property_map(OuterMap&& outer, InnerMap&& inner, OtherMaps&&... other) {
    return make_composite_property_map(
        composite_property_map<OuterMap, InnerMap>(std::forward<OuterMap>(outer),
                                                   std::forward<InnerMap>(inner)),
        std::forward<OtherMaps>(other)...);
}

}  // namespace ac
