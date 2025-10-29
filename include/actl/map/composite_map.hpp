// composite_map combines multiple maps, applying them in the given order.
//
// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/map/traits.hpp>
#include <actl/memory/AC_NO_UNIQUE_ADDRESS.hpp>
#include <actl/range/iterator/transform_iterator.hpp>
#include <actl/range/iterator_range.hpp>

namespace ac {

namespace detail {

template<typename M1, typename M2, typename V, bool I1, bool I2>
struct cm_range {
    struct get2 {
        // Pointer is used instead of a reference to support copy assignment
        // required for std::copyable.
        M2* map2;

        V operator()(map_pair_t<M1> p1) const {
            return {p1.first, ac::get(*map2, p1.second)};
        }
    };

    using type = iterator_range<transform_iterator<map_iterator_t<M1>, get2>>;
};

template<typename M1, typename M2, typename V>
struct cm_range<M1, M2, V, false, true> {
    struct invert1 {
        // Pointer is used instead of a reference to support copy assignment
        // required for std::copyable.
        M1* map1;

        V operator()(map_pair_t<M2> p2) const {
            return {ac::invert(*map1, p2.first), p2.second};
        }
    };

    using type =
        iterator_range<transform_iterator<map_iterator_t<M2>, invert1>>;
};

template<typename M1, typename M2, typename V>
struct cm_range<M1, M2, V, false, false> {
    using type = void;
};

} // namespace detail

template<typename Map1, typename Map2, typename... Maps>
class composite_map
    : public composite_map<composite_map<Map1, Map2>, Maps...> {};

template<typename M1, typename M2>
class composite_map<M1, M2> {
    using K = map_key_t<M1>;
    using R = map_reference_t<M2>;

public:
    M1 map1;
    M2 map2;

    static_assert(
        std::is_convertible_v<map_reference_t<M1>, map_key_t<M2>>,
        "incompatible property maps"
    );

    static constexpr bool writable2 =
        map_traits<M1>::readable && map_traits<M2>::writable;
    static constexpr bool iterable1 =
        map_traits<M1>::iterable && map_traits<M2>::readable;
    static constexpr bool iterable2 =
        map_traits<M1>::invertible && map_traits<M2>::iterable;
    using range_t = typename detail::
        cm_range<M1, M2, std::pair<K, R>, iterable1, iterable2>::type;

    struct is_composite_map;

    using traits = map_traits_base<
        K,
        R,
        map_value_t<M2>,
        map_traits<M1>::readable && map_traits<M2>::readable,
        writable2 || (map_traits<M1>::writable && map_traits<M2>::invertible),
        map_traits<M1>::invertible && map_traits<M2>::invertible,
        iterable1 || iterable2,
        range_t>;
};

template<typename... Ms>
struct map_traits<composite_map<Ms...>> : composite_map<Ms...>::traits {};

template<typename... Ms>
struct map_traits<composite_map<Ms...> const>
    : map_traits<composite_map<Ms const...>> {};

template<typename CM>
    requires requires { typename CM::is_composite_map; }
struct map_ops<CM> {
    using K = map_key_t<CM>;
    using V = map_value_t<CM>;

    static constexpr map_reference_t<CM> get(CM& map, K key) {
        return ac::get(map.map2, ac::get(map.map1, key));
    }

    static constexpr void put(CM& map, K key, V const& value) {
        if constexpr (CM::writable2) {
            ac::put(map.map2, ac::get(map.map1, key), value);
        } else {
            ac::put(map.map1, key, ac::invert(map.map2, value));
        }
    }

    static constexpr K invert(CM& map, V const& value) {
        return ac::invert(map.map1, ac::invert(map.map2, value));
    }

    static constexpr map_range_t<CM> map_range(CM& map) {
        if constexpr (CM::iterable1) {
            map_range_t<decltype(map.map1)> r = ac::map_range(map.map1);
            return {{r.begin(), &map.map2}, {r.end(), &map.map2}};
        } else if constexpr (CM::iterable2) {
            map_range_t<decltype(map.map2)> r = ac::map_range(map.map2);
            return {{r.begin(), &map.map1}, {r.end(), &map.map1}};
        }
    }
};

template<typename... Maps>
composite_map(Maps&&...) -> composite_map<Maps...>;

} // namespace ac
