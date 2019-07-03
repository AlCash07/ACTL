/***************************************************************************************************
 * composite_map combines multiple maps, applying them in the given order.
 ***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/iterator/iterator_adaptor.hpp>
#include <actl/map/traits.hpp>
#include <actl/range/iterator_range.hpp>
#include <actl/util/compressed_pair.hpp>

namespace ac {

namespace detail {

template <class M1, class M2, class V, bool I1, bool I2>
struct cm_range {
    using It1 = map_iterator_t<M1>;

    class iterator : public iterator_adaptor<iterator, It1, use_default, V, V, V*> {
        friend struct ac::iterator_core_access;

        V dereference() const { return {this->base()->first, ac::get(map_, this->base()->second)}; }

        M2& map_;

    public:
        iterator(const It1& it, M2& map)
            : iterator_adaptor<iterator, It1, use_default, V, V, V*>{it}, map_{map} {}
    };

    using type = iterator_range<iterator>;
};

template <class M1, class M2, class V>
struct cm_range<M1, M2, V, false, true> {
    using It2 = map_iterator_t<M2>;

    class iterator : public iterator_adaptor<iterator, It2, use_default, V, V, V*> {
        friend struct ac::iterator_core_access;

        V dereference() const {
            return {ac::invert(map_, this->base()->first), this->base()->second};
        }

        M1& map_;

    public:
        iterator(const It2& it, M1& map)
            : iterator_adaptor<iterator, It2, use_default, V, V, V*>{it}, map_{map} {}
    };

    using type = iterator_range<iterator>;
};

template <class M1, class M2, class V>
struct cm_range<M1, M2, V, false, false> {
    using type = void;
};

}  // namespace detail

template <class Map1, class Map2, class... Maps>
class composite_map : public composite_map<composite_map<Map1, Map2>, Maps...> {};

template <class M1, class M2>
class composite_map<M1, M2> : public compressed_pair<M1, M2> {
public:
    static_assert(std::is_convertible_v<map_reference_t<M1>, map_key_t<M2>>,
                  "incompatible property maps");

    struct is_composite_map;

    using compressed_pair<M1, M2>::compressed_pair;
};

template <class CM>
class map_traits<CM, std::void_t<typename CM::is_composite_map>> {
    using M1 = decltype(std::declval<CM>().first());
    using M2 = decltype(std::declval<CM>().second());

    static constexpr bool writable2 = map_traits<M1>::readable && map_traits<M2>::writable;
    static constexpr bool iterable1 = map_traits<M1>::iterable && map_traits<M2>::readable;
    static constexpr bool iterable2 = map_traits<M1>::invertible && map_traits<M2>::iterable;

public:
    static constexpr bool readable = map_traits<M1>::readable && map_traits<M2>::readable;
    static constexpr bool writable = writable2 || (map_traits<M1>::writable && map_traits<M2>::invertible);
    static constexpr bool invertible = map_traits<M1>::invertible && map_traits<M2>::invertible;
    static constexpr bool iterable = iterable1 || iterable2;

    using key_type = map_key_t<M1>;
    using value_type = map_value_t<M2>;
    using reference = map_reference_t<M2>;
    using range_type = typename detail::cm_range<M1, M2, std::pair<key_type, reference>, iterable1, iterable2>::type;

    static constexpr reference get(CM& map, key_type key) {
        return ac::get(map.second(), ac::get(map.first(), key));
    }

    static constexpr void put(CM& map, key_type key, value_type value) {
        if constexpr (writable2) {
            ac::put(map.second(), ac::get(map.first(), key), value);
        } else {
            ac::put(map.first(), key, ac::invert(map.second(), value));
        }
    }

    static constexpr key_type invert(CM& map, value_type value) {
        return ac::invert(map.first(), ac::invert(map.second(), value));
    }

    static constexpr range_type map_range(CM& map) {
        if constexpr (iterable1) {
            map_range_t<M1> r = ac::map_range(map.first());
            return {{r.begin(), map.second()}, {r.end(), map.second()}};
        } else if constexpr (iterable2) {
            map_range_t<M2> r = ac::map_range(map.second());
            return {{r.begin(), map.first()}, {r.end(), map.first()}};
        }
    }
};

template <class... Maps>
composite_map(Maps&&...) -> composite_map<Maps...>;

}  // namespace ac
