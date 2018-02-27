/***************************************************************************************************
 * Traits and generalized functions for standard containers.
 ***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <list>
#include <set>
#include <type_traits>
#include <unordered_set>
#include <utility>
#include <vector>

namespace ac {

/* Container tags */

struct container_tag {};

struct sequence_tag : container_tag {};
struct random_access_tag : sequence_tag {};

struct associative_tag : container_tag {};
struct unique_associative_tag : associative_tag {};
struct multiple_associative_tag : associative_tag {};

template <class T>
struct container_traits;

template <class T>
struct container_traits<const T> : container_traits<T> {};

template <class T>
struct container_traits<std::vector<T>> {
    using category = random_access_tag;

    template <class U>
    using type = std::vector<U>;
};

template <class T>
struct container_traits<std::list<T>> {
    using category = sequence_tag;

    template <class U>
    using type = std::list<U>;
};

template <class T>
struct container_traits<std::set<T>> {
    using category = unique_associative_tag;

    template <class U>
    using type = std::set<U>;
};

template <class T>
struct container_traits<std::multiset<T>> {
    using category = multiple_associative_tag;

    template <class U>
    using type = std::multiset<U>;
};

template <class T>
struct container_traits<std::unordered_set<T>> {
    using category = unique_associative_tag;

    template <class U>
    using type = std::unordered_set<U>;
};

template <class T>
struct container_traits<std::unordered_multiset<T>> {
    using category = multiple_associative_tag;

    template <class U>
    using type = std::unordered_multiset<U>;
};

template <class C, class Tag>
struct has_tag : std::is_base_of<Tag, typename container_traits<C>::category> {};

template <class C> struct is_sequence             : has_tag<C, sequence_tag> {};
template <class C> struct is_random_access        : has_tag<C, random_access_tag> {};
template <class C> struct is_associative          : has_tag<C, associative_tag> {};
template <class C> struct is_unique_associative   : has_tag<C, unique_associative_tag> {};
template <class C> struct is_multiple_associative : has_tag<C, multiple_associative_tag> {};

/* Container descriptor : int for random access containers, iterator otherwise */

template <class C, bool = is_random_access<C>::value>
struct container_descriptor {
    using type = typename C::iterator;
};

template <class C>
struct container_descriptor<C, true> {
    using type = int;
};

template <class C>
using container_descriptor_t = typename container_descriptor<C>::type;

// Does erasure invalidate container iterators and descriptors except the erased one.
template <class C>
struct is_stable : std::bool_constant<!is_random_access<C>::value> {};

/* Generalized functions */

// Access by descriptor
template <class C>
inline typename std::conditional_t<std::is_const_v<C>, typename C::const_reference,
                                   typename C::reference>
get_reference(C& c, container_descriptor_t<C> d) {
    if constexpr (is_random_access<C>::value) {
        return c[d];
    } else {
        return *d;
    }
}

// Push
template <class C, class T>
inline std::pair<container_descriptor_t<C>, bool> push(C& c, T&& v) {
    if constexpr (is_associative<C>::value) {
        auto res = c.insert(std::forward<T>(v));
        if constexpr (is_unique_associative<C>::value) {
            return res;
        } else {
            return {res, true};
        }
    } else if constexpr (is_random_access<C>::value) {
        c.push_back(std::forward<T>(v));
        return {static_cast<int>(c.size()) - 1, true};
    } else {
        return {c.insert(c.end(), std::forward<T>(v)), true};
    }
}

}  // namespace ac
