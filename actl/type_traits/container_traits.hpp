/***************************************************************************************************
 * Traits and generalized functions for standard containers.
 ***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <cstdint>
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

template <class C>
struct container_traits;

template <class C>
struct container_traits<const C> : container_traits<C> {};

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

/* Container type traits */

namespace detail {

template <class CT, class = void>
struct is_container_impl : std::false_type {};

template <class CT>
struct is_container_impl<CT, std::void_t<typename CT::category>> : std::true_type {};

template <class C, class Tag, bool = true>
struct has_container_tag_impl : std::is_base_of<Tag, typename container_traits<C>::category> {};

template <class C, class Tag>
struct has_container_tag_impl<C, Tag, false> : std::false_type {};

}  // namespace detail

template <class C>
struct is_container : detail::is_container_impl<container_traits<C>> {};

template <class C, class Tag>
struct has_container_tag : detail::has_container_tag_impl<C, Tag, is_container<C>::value> {};

template <class C> struct is_sequence             : has_container_tag<C, sequence_tag> {};
template <class C> struct is_random_access        : has_container_tag<C, random_access_tag> {};
template <class C> struct is_associative          : has_container_tag<C, associative_tag> {};
template <class C> struct is_unique_associative   : has_container_tag<C, unique_associative_tag> {};
template <class C> struct is_multiple_associative : has_container_tag<C, multiple_associative_tag> {
};

// Does erasure invalidate container iterators except the erased one.
template <class C>
struct is_stable : std::bool_constant<!is_random_access<C>::value> {};

/* Container ID: int for random access containers, iterator otherwise. */

template <class C, bool = is_random_access<C>::value>
struct container_id {
    using type = typename C::const_iterator;
};

template <class C>
struct container_id<C, true> {
    using type = int;
};

template <class C>
using container_id_t = typename container_id<C>::type;

// ID key that can be used for comparison and hashing.
constexpr auto get_id_key(int value) { return value; }

template <class ID>
constexpr inline auto get_id_key(ID id) {
    return reinterpret_cast<std::uintptr_t>(&*id);
}

/* Generalized functions */

// void_id: returns an invalid ID that is fixed for the given container.
template <class C>
inline container_id_t<C> void_id(const C& c) {
    if constexpr (is_random_access<C>::value) {
        return -1;
    } else {
        return c.end();
    }
}

// get_reference: element access by ID.
template <class C>
inline typename C::const_reference get_reference(const C& c, container_id_t<C> id) {
    if constexpr (is_random_access<C>::value) {
        return c[id];
    } else {
        return *id;
    }
}

template <class C>
inline typename C::reference get_reference(C& c, container_id_t<C> id) {
    if constexpr (is_random_access<C>::value) {
        return c[id];
    } else {
        // const_cast is required because id is a const_iterator.
        return const_cast<typename C::reference>(*id);
    }
}

// find by value.
template <class C, class T>
inline container_id_t<C> find(const C& c, const T& value) {
    if constexpr (is_associative<C>::value) {
        return c.find(value);
    } else {
        auto it = std::find(c.begin(), c.end(), value);
        if constexpr (is_random_access<C>::value) {
            return it == c.end() ? void_id(c) : static_cast<int>(it - c.begin());
        } else {
            return it;
        }
    }
}

// emplace. Doesn't invalidate IDs.
template <class C, class... Ts>
inline std::pair<container_id_t<C>, bool> emplace(C& c, Ts&&... args) {
    if constexpr (is_associative<C>::value) {
        auto res = c.emplace(std::forward<Ts>(args)...);
        if constexpr (is_unique_associative<C>::value) {
            return res;
        } else {
            return {res, true};
        }
    } else if constexpr (is_random_access<C>::value) {
        c.emplace_back(std::forward<Ts>(args)...);
        return {static_cast<int>(c.size()) - 1, true};
    } else {
        return {c.emplace(c.end(), std::forward<Ts>(args)...), true};
    }
}

// erase by ID.
template <class C>
inline void erase(C& c, container_id_t<C> id) {
    if constexpr (is_random_access<C>::value) {
        c.erase(c.begin() + id);
    } else {
        c.erase(id);
    }
}

}  // namespace ac
