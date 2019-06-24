/***************************************************************************************************
 * composite_map combines multiple maps, applying them in the given order.
 * Supports put, inversion and traversal if at all possible.
 ***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/iterator/iterator_adaptor.hpp>
#include <actl/map/property_map.hpp>
#include <actl/std/utility.hpp>
#include <actl/util/compressed_pair.hpp>
#include <type_traits>

namespace ac {

namespace detail {

template <class Map1, class Map2>
class composite_map_base : property_map_base, protected compressed_pair<Map1, Map2> {
    using base_t = compressed_pair<Map1, Map2>;

public:
    using base_t::base_t;
};

// This type is used to avoid multiple inheritance from property_map_base.
template <class Map1, class Map2>
using composite_map_base_t =
    std::conditional_t<std::is_base_of_v<property_map_base, compressed_pair<Map1, Map2>>,
                       compressed_pair<Map1, Map2>, composite_map_base<Map1, Map2>>;

// Class that adds typedefs and get function.
template <class Map1, class Map2>
class composite_map_get : public composite_map_base_t<Map1, Map2> {
public:
    static_assert(std::is_convertible_v<typename map_traits<Map1>::reference,
                                        typename map_traits<Map2>::key_type>,
                  "incompatible property maps");

    using key_type = typename map_traits<Map1>::key_type;
    using value_type = typename map_traits<Map2>::value_type;
    using reference = typename map_traits<Map2>::reference;

    using composite_map_base_t<Map1, Map2>::composite_map_base_t;

    friend constexpr reference get(const composite_map_get& map, key_type key) {
        return get(map.second(), get(map.first(), key));
    }
};

// Class that adds put function if possible.
template <class Map1, class Map2, bool, bool, bool>
class composite_map_put : public composite_map_get<Map1, Map2> {
public:
    static constexpr bool writable = false;

    using composite_map_get<Map1, Map2>::composite_map_get;
};

template <class Map1, class Map2, bool W1, bool Inv2>
class composite_map_put<Map1, Map2, W1, true, Inv2> : public composite_map_get<Map1, Map2> {
    using base_t = composite_map_get<Map1, Map2>;

public:
    static constexpr bool writable = true;

    using base_t::base_t;

    friend constexpr void put(const composite_map_put& map, typename base_t::key_type key,
                              typename base_t::value_type value) {
        put(map.second(), get(map.first(), key), value);
    }
};

template <class Map1, class Map2>
class composite_map_put<Map1, Map2, true, false, true> : public composite_map_get<Map1, Map2> {
    using base_t = composite_map_get<Map1, Map2>;

public:
    static constexpr bool writable = true;

    using base_t::base_t;

    friend constexpr void put(const composite_map_put& map, typename base_t::key_type key,
                              typename base_t::value_type value) {
        put(map.first(), key, map.second().invert(value));
    }
};

template <class Map1, class Map2>
using composite_map_put_t =
    composite_map_put<Map1, Map2, map_traits<Map1>::writable, map_traits<Map2>::writable,
                      map_traits<Map2>::invertible>;

// Class that adds invert method if possible.
template <class Map1, class Map2, bool>
class composite_map_invert : public composite_map_put_t<Map1, Map2> {
public:
    static constexpr bool invertible = false;

    using composite_map_put_t<Map1, Map2>::composite_map_put_t;
};

template <class Map1, class Map2>
class composite_map_invert<Map1, Map2, true> : public composite_map_put_t<Map1, Map2> {
    using base_t = composite_map_put_t<Map1, Map2>;

public:
    static constexpr bool invertible = true;

    using base_t::base_t;

    constexpr typename base_t::key_type invert(typename base_t::value_type value) const {
        return this->first().invert(this->second().invert(value));
    }
};

template <class Map1, class Map2>
using composite_map_invert_t =
    composite_map_invert<Map1, Map2, map_traits<Map1>::invertible && map_traits<Map2>::invertible>;

// Class that adds begin/end methods if possible.
template <class Map1, class Map2, bool, bool, bool>
class composite_map_iterate : public composite_map_invert_t<Map1, Map2> {
public:
    static constexpr bool iterable = false;

    using composite_map_invert_t<Map1, Map2>::composite_map_invert_t;
};

template <class Map1, class Map2, bool It2, bool Inv2>
class composite_map_iterate<Map1, Map2, true, It2, Inv2>
    : public composite_map_invert_t<Map1, Map2> {
    using base_t = composite_map_invert_t<Map1, Map2>;
    using It = typename Map1::iterator;
    using Pair = std::pair<typename base_t::key_type, typename base_t::reference>;

public:
    class iterator
        : public iterator_adaptor<iterator, It, use_default, Pair, Pair, Pair*, use_default> {
        iterator(const It& it, const Map2& map)
            : iterator_adaptor<iterator, It, use_default, Pair, Pair, Pair*, use_default>{it}
            , map_{map} {}

        Pair dereference() const { return {this->base()->first, get(map_, this->base()->second)}; }

        const Map2& map_;

        friend class composite_map_iterate;
        friend struct ac::iterator_core_access;
    };

    static constexpr bool iterable = true;

    using base_t::base_t;

    iterator begin() const { return {this->first().begin(), this->second()}; }
    iterator end() const { return {this->first().end(), this->second()}; }
};

template <class Map1, class Map2>
class composite_map_iterate<Map1, Map2, false, true, true>
    : public composite_map_invert_t<Map1, Map2> {
    using base_t = composite_map_invert_t<Map1, Map2>;
    using It = typename Map2::iterator;
    using Pair = std::pair<typename base_t::key_type, typename base_t::reference>;

public:
    class iterator
        : public iterator_adaptor<iterator, It, use_default, Pair, Pair, Pair*, use_default> {
        iterator(const It& it, const Map1& map)
            : iterator_adaptor<iterator, It, use_default, Pair, Pair, Pair*, use_default>{it}
            , map_{map} {}

        Pair dereference() const {
            return {map_.invert(this->base()->first), this->base()->second};
        }

        const Map1& map_;

        friend class composite_map_iterate;
        friend struct ac::iterator_core_access;
    };

    static constexpr bool iterable = true;

    using base_t::base_t;

    iterator begin() const { return {this->second().begin(), this->first()}; }
    iterator end() const { return {this->second().end(), this->first()}; }
};

template <class Map1, class Map2>
using composite_map_t =
    composite_map_iterate<Map1, Map2, map_traits<Map1>::iterable, map_traits<Map2>::iterable,
                          map_traits<Map1>::invertible>;

}  // namespace detail

template <class Map1, class Map2, class... Maps>
class composite_map : public composite_map<composite_map<Map1, Map2>, Maps...> {};

template <class Map1, class Map2>
class composite_map<Map1, Map2> : public detail::composite_map_t<Map1, Map2> {
    using base_t = detail::composite_map_t<Map1, Map2>;

public:
    using base_t::base_t;
};

template <class... Maps>
composite_map(Maps&&...) -> composite_map<Maps...>;

}  // namespace ac
