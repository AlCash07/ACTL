// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/map/generic_container.hpp>
#include <actl/memory/AC_NO_UNIQUE_ADDRESS.hpp>
#include <actl/range/filtered_range.hpp>

namespace ac {

struct to_bool {
    template<class T>
    constexpr bool operator()(T const& x) const {
        return static_cast<bool>(x);
    }
};

template<class Pred>
struct test_second {
    AC_NO_UNIQUE_ADDRESS Pred predicate;

    template<class Pair>
    constexpr bool operator()(Pair const& x) const {
        return predicate(x.second);
    }
};

/// Container property map with traversal interface that skips values not
/// satisfying predicate.
template<class Map, class Predicate = to_bool>
class filtered_map {
public:
    template<class MapT, class... PredArgs>
    explicit constexpr filtered_map(MapT&& map, PredArgs&&... xs)
        : m_map{std::forward<MapT>(map)}
        , m_pred{std::forward<PredArgs>(xs)...} {}

    operator Map&() {
        return m_map;
    }

    operator Map const&() const {
        return m_map;
    }

    auto map_range() {
        return filter_range(ac::map_range(m_map), m_pred);
    }

    auto map_range() const {
        return filter_range(ac::map_range(m_map), m_pred);
    }

private:
    Map m_map;
    test_second<Predicate> m_pred;
};

template<class M, class P = to_bool>
filtered_map(M&&, P = {}) -> filtered_map<M, P>;

template<class M, class P>
struct map_traits<filtered_map<M, P>> : map_traits<M> {
    using range_type = filtered_range<map_range_t<M>, test_second<P>>;
};

template<class M, class P>
struct map_traits<filtered_map<M, P> const>
    : map_traits<filtered_map<M const, P>> {};

template<class M, class P>
struct map_ops<filtered_map<M, P>> : map_ops<M> {
    static auto map_range(filtered_map<M, P>& map) {
        return map.map_range();
    }
};

template<class M, class P>
struct map_ops<filtered_map<M, P> const> : map_ops<M const> {
    static auto map_range(filtered_map<M, P> const& map) {
        return map.map_range();
    }
};

/// Transition property map with underlying fixed-size array.
template<class T, int N, class Predicate>
using filtered_array_map = filtered_map<std::array<T, N>, Predicate>;

} // namespace ac
