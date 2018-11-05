/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/property_map/container_property_map.hpp>
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
class transition_property_map
    : public put_helper<transition_property_map<Container, Predicate>>,
      public filtered_range<container_property_map<Container>, test_second<Predicate>> {
    using CPM = container_property_map<Container>;
    using TSP = test_second<Predicate>;

public:
    template <class C>
    explicit transition_property_map(C&& cont, Predicate pred)
        : filtered_range<CPM, TSP>(CPM(std::forward<C>(cont)), TSP(pred)) {}

    friend typename CPM::reference get(const transition_property_map& pm,
                                       typename CPM::key_type         key) {
        return get(pm.original(), key);
    }
};

template <class Container, class Predicate = to_bool>
inline auto make_transition_property_map(Container&& cont, Predicate pred = {}) {
    return transition_property_map<remove_rvalue_ref_t<Container>, Predicate>(
        std::forward<Container>(cont), pred);
}

template <class C, class P>
struct property_traits<transition_property_map<C, P>> : property_traits<container_property_map<C>> {
};

/**
 * Transition property map with underlying fixed-size array.
 */
template <class T, int N, class Predicate>
using transition_array_property_map = transition_property_map<std::array<T, N>, Predicate>;

}  // namespace ac
