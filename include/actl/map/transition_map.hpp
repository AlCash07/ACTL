/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/map/container_map.hpp>
#include <actl/range/filtered_range.hpp>
#include <actl/util/compressed_pair.hpp>

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

/**
 * Container property map with traversal interface that skips values not satisfying predicate.
 */
template <class Container, class Predicate>
class transition_map : public put_helper<transition_map<Container, Predicate>>,
                       public filtered_range<container_map<Container>, test_second<Predicate>> {
    using CM = container_map<Container>;
    using TSP = test_second<Predicate>;

public:
    template <class C>
    explicit transition_map(C&& cont, Predicate pred = {})
        : filtered_range<CM, TSP>{CM{std::forward<C>(cont)}, TSP{pred}} {}

    friend typename CM::reference get(const transition_map& map, typename CM::key_type key) {
        return get(map.original(), key);
    }
};

template <class C, class P = to_bool>
transition_map(C&&, P = {}) -> transition_map<remove_rvalue_ref_t<C>, P>;

template <class C, class P>
struct map_traits<transition_map<C, P>> : map_traits<container_map<C>> {};

/**
 * Transition property map with underlying fixed-size array.
 */
template <class T, int N, class Predicate>
using transition_array_map = transition_map<std::array<T, N>, Predicate>;

}  // namespace ac