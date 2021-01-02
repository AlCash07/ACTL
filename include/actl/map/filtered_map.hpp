// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/map/generic_container.hpp>
#include <actl/range/filtered_range.hpp>
#include <actl/utility/compressed_pair.hpp>

namespace ac {

struct to_bool {
    template <class T>
    constexpr bool operator()(const T& x) const {
        return static_cast<bool>(x);
    }
};

template <class Pred>
struct test_second : public ebo<Pred> {
    using ebo<Pred>::ebo;

    template <class Pair>
    constexpr bool operator()(const Pair& x) const {
        return this->get()(x.second);
    }
};

/// Container property map with traversal interface that skips values not satisfying predicate.
template <class Map, class Predicate = to_bool>
class filtered_map : private compressed_pair<Map, test_second<Predicate>> {
    using base_t = compressed_pair<Map, test_second<Predicate>>;

    using base_t::first;
    using base_t::second;

public:
    using base_t::base_t;

    operator Map&() {
        return first();
    }
    operator const Map&() const {
        return first();
    }

    auto map_range() {
        return filter_range(ac::map_range(first()), second());
    }
    auto map_range() const {
        return filter_range(ac::map_range(first()), second());
    }
};

template <class M, class P = to_bool>
filtered_map(M&&, P = {}) -> filtered_map<M, P>;

template <class M, class P>
struct map_traits<filtered_map<M, P>> : map_traits<M> {
    using range_type = filtered_range<map_range_t<M>, test_second<P>>;
};

template <class M, class P>
struct map_traits<const filtered_map<M, P>> : map_traits<filtered_map<const M, P>> {};

template <class M, class P>
struct map_ops<filtered_map<M, P>> : map_ops<M> {
    static auto map_range(filtered_map<M, P>& map) {
        return map.map_range();
    }
};

template <class M, class P>
struct map_ops<const filtered_map<M, P>> : map_ops<const M> {
    static auto map_range(const filtered_map<M, P>& map) {
        return map.map_range();
    }
};

/// Transition property map with underlying fixed-size array.
template <class T, int N, class Predicate>
using filtered_array_map = filtered_map<std::array<T, N>, Predicate>;

}  // namespace ac
