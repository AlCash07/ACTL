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

#include <actl/iterator/iterator_adaptor.hpp>
#include <actl/property_map/property_map.hpp>
#include <actl/type/compressed_pair.hpp>
#include <type_traits>
#include <utility>

namespace ac {

namespace detail {

// Base class containing the maps and implementing common methods.
template <class M1, class M2>
class composite_pm_base : public property_map_base, protected compressed_pair<M1, M2> {
public:
    static_assert(std::is_convertible_v<typename property_traits<M1>::const_reference,
                                        const typename property_traits<M2>::key_type&>,
                  "incompatible maps");

    using key_type        = typename property_traits<M1>::key_type;
    using value_type      = typename property_traits<M2>::value_type;
    using reference       = typename property_traits<M2>::reference;
    using const_reference = typename property_traits<M2>::const_reference;

    // TODO: such definition for writeable is incorrect in some cases.
    static constexpr bool writeable = property_traits<M2>::writeable;

    template <class... Ts>
    explicit constexpr composite_pm_base(Ts&&... args)
        : compressed_pair<M1, M2>(std::forward<Ts>(args)...) {}

    constexpr reference operator[](key_type& key) {
        return this->second()[this->first()[key]];
    }

    constexpr const_reference operator[](const key_type& key) const {
        return this->second()[this->first()[key]];
    }

    void clear() {
        this->first().clear();
        this->second().clear();
    }
};

// Class that adds invert method if possible.
template <class M1, class M2, bool, bool>
class composite_pm_invert : public composite_pm_base<M1, M2> {
public:
    static constexpr bool invertible = false;

    using composite_pm_base<M1, M2>::composite_pm_base;
};

template <class M1, class M2>
class composite_pm_invert<M1, M2, true, true> : public composite_pm_base<M1, M2> {
    using base_t = composite_pm_base<M1, M2>;

public:
    static constexpr bool invertible = true;

    using base_t::base_t;

    constexpr const typename base_t::key_type invert(
        const typename base_t::value_type& value) const {
        return this->first().invert(this->second().invert(value));
    }
};

// Middle class to simplify inheritance declarations.
template <class M1, class M2, bool I>
class composite_pm_mid : public composite_pm_invert<M1, M2, I, property_traits<M2>::invertible> {
public:
    using composite_pm_invert<M1, M2, I, property_traits<M2>::invertible>::composite_pm_invert;
};

// Class that adds begin/end methods if possible.
template <class M1, class M2, bool, bool, bool I>
class composite_pm_impl : public composite_pm_mid<M1, M2, I> {
public:
    static constexpr bool traversible = false;

    using composite_pm_mid<M1, M2, I>::composite_pm_mid;
};

template <class Iterator, class M2, class Key, class Ref, class Pair = std::pair<Key, Ref>>
class pm_iterator : public iterator_adaptor<pm_iterator<Iterator, M2, Key, Ref, Pair>, Iterator,
                                            use_default, Pair, Pair, Pair*, use_default> {
public:
    pm_iterator(const Iterator& it, M2& map)
        : iterator_adaptor<pm_iterator<Iterator, M2, Key, Ref, Pair>, Iterator, use_default, Pair,
                           Pair, Pair*, use_default>(it),
          map_{map} {}

private:
    friend struct ac::iterator_core_access;

    Pair dereference() const { return {this->base()->first, map_[this->base()->second]}; }

    M2& map_;
};

template <class M1, class M2, bool T, bool I>
class composite_pm_impl<M1, M2, true, T, I> : public composite_pm_mid<M1, M2, I> {
    using base_t = composite_pm_mid<M1, M2, I>;

public:
    using iterator = pm_iterator<typename M1::iterator,
                                 M2,
                                 typename M1::key_type,
                                 typename property_traits<M2>::reference>;

    using const_iterator = pm_iterator<typename M1::const_iterator,
                                       const M2,
                                       typename M1::key_type,
                                       typename property_traits<M2>::const_reference>;

    static constexpr bool traversible = true;

    using base_t::base_t;

    iterator begin() { return {this->first().begin(), this->second()}; }
    iterator end()   { return {this->first().end(),   this->second()}; }

    const_iterator begin() const { return {this->first().begin(), this->second()}; }
    const_iterator end()   const { return {this->first().end(),   this->second()}; }
};

template <class Iterator, class M1, class Key, class Ref, class Pair = std::pair<Key, Ref>>
class pm_invert_iterator
    : public iterator_adaptor<pm_invert_iterator<Iterator, M1, Key, Ref, Pair>, Iterator,
                              use_default, Pair, Pair, Pair*, use_default> {
public:
    pm_invert_iterator(const Iterator& it, M1& map)
        : iterator_adaptor<pm_invert_iterator<Iterator, M1, Key, Ref, Pair>, Iterator, use_default,
                           Pair, Pair, Pair*, use_default>(it),
          map_{map} {}

private:
    friend struct ac::iterator_core_access;

    Pair dereference() const { return {map_.invert(this->base()->first), this->base()->second}; }

    M1& map_;
};

template <class M1, class M2>
class composite_pm_impl<M1, M2, false, true, true> : public composite_pm_mid<M1, M2, true> {
    using base_t = composite_pm_mid<M1, M2, true>;

public:
    using iterator = pm_invert_iterator<typename M2::iterator,
                                        M1,
                                        typename property_traits<M1>::key_type,
                                        typename M2::reference>;

    using const_iterator = pm_invert_iterator<typename M2::const_iterator,
                                              const M1,
                                              typename property_traits<M1>::key_type,
                                              typename M2::const_reference>;

    static constexpr bool traversible = true;

    using base_t::base_t;

    iterator begin() { return {this->second().begin(), this->first()}; }
    iterator end()   { return {this->second().end(),   this->first()}; }

    const_iterator begin() const { return {this->second().begin(), this->first()}; }
    const_iterator end()   const { return {this->second().end(),   this->first()}; }
};

// Type alias to simplify inheritance declarations.
template <class M1, class M2>
using composite_pm_impl_t =
    composite_pm_impl<M1, M2, property_traits<M1>::traversible, property_traits<M2>::traversible,
                      property_traits<M1>::invertible>;

}  // namespace detail

template <class Map1, class Map2, class... Maps>
class composite_property_map
    : public composite_property_map<composite_property_map<Map1, Map2>, Maps...> {};

template <class Map1, class Map2>
class composite_property_map<Map1, Map2> : public detail::composite_pm_impl_t<Map1, Map2> {
    using base_t = detail::composite_pm_impl_t<Map1, Map2>;

public:
    using base_t::base_t;
};

template <class Map>
inline Map make_composite_property_map(Map&& map) { return map; }

template <class Map1, class Map2, class... Maps>
inline auto make_composite_property_map(Map1&& map1, Map2&& map2, Maps&&... maps) {
    return make_composite_property_map(
        composite_property_map<Map1, Map2>(std::forward<Map1>(map1), std::forward<Map2>(map2)),
        std::forward<Maps>(maps)...);
}

}  // namespace ac
