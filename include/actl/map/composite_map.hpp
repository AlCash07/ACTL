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
    using K = map_key_t<M1>;
    using R = map_reference_t<M2>;

public:
    static_assert(std::is_convertible_v<map_reference_t<M1>, map_key_t<M2>>,
                  "incompatible property maps");

    static constexpr bool writable2 = map_traits<M1>::readable && map_traits<M2>::writable;
    static constexpr bool iterable1 = map_traits<M1>::iterable && map_traits<M2>::readable;
    static constexpr bool iterable2 = map_traits<M1>::invertible && map_traits<M2>::iterable;
    using range_t = typename detail::cm_range<M1, M2, std::pair<K, R>, iterable1, iterable2>::type;

    struct is_composite_map;

    using traits =
        map_traits_base<K, R, map_value_t<M2>, map_traits<M1>::readable && map_traits<M2>::readable,
                        writable2 || (map_traits<M1>::writable && map_traits<M2>::invertible),
                        map_traits<M1>::invertible && map_traits<M2>::invertible,
                        iterable1 || iterable2, range_t>;

    using compressed_pair<M1, M2>::compressed_pair;
};

template <class... Ms>
struct map_traits<composite_map<Ms...>> : composite_map<Ms...>::traits {};

template <class... Ms>
struct map_traits<const composite_map<Ms...>> : map_traits<composite_map<const Ms...>> {};

template <class CM>
struct map_ops<CM, std::void_t<typename CM::is_composite_map>> {
    using K = map_key_t<CM>;
    using V = map_value_t<CM>;

    static constexpr map_reference_t<CM> get(CM& map, K key) {
        return ac::get(map.second(), ac::get(map.first(), key));
    }

    static constexpr void put(CM& map, K key, V value) {
        if constexpr (CM::writable2) {
            ac::put(map.second(), ac::get(map.first(), key), value);
        } else {
            ac::put(map.first(), key, ac::invert(map.second(), value));
        }
    }

    static constexpr K invert(CM& map, V value) {
        return ac::invert(map.first(), ac::invert(map.second(), value));
    }

    static constexpr map_range_t<CM> map_range(CM& map) {
        if constexpr (CM::iterable1) {
            map_range_t<typename CM::first_type> r = ac::map_range(map.first());
            return {{r.begin(), map.second()}, {r.end(), map.second()}};
        } else {
            map_range_t<typename CM::second_type> r = ac::map_range(map.second());
            return {{r.begin(), map.first()}, {r.end(), map.first()}};
        }
    }
};

template <class... Maps>
composite_map(Maps&&...) -> composite_map<Maps...>;

}  // namespace ac
