/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/assert.hpp>
#include <actl/container/functions.hpp>
#include <actl/hash.hpp>
#include <actl/iterator/integer_iterator.hpp>
#include <actl/iterator/iterator_adaptor.hpp>
#include <actl/range/iterator_range.hpp>
#include <actl/traits/container_traits.hpp>
#include <actl/util/bit.hpp>
#include <cstdint>

namespace ac {

template <class C, bool = is_container_v<C>, bool = is_random_access_container_v<C>>
struct container_id_traits;

template <class It>
class iterator_id : public iterator_adaptor<iterator_id<It>, It, use_default, iterator_id<It>,
                                            iterator_id<It>, iterator_id<It>*> {
    using id = iterator_id;

public:
    explicit iterator_id(It it = It{})
        : iterator_adaptor<iterator_id<It>, It, use_default, id, id, id*>(it) {}

    explicit iterator_id(void* raw)
        : iterator_adaptor<iterator_id<It>, It, use_default, id, id, id*>(bit_cast<It>(raw)) {
        // TODO: implement more general logic in case this condition fails (very unlikely).
        static_assert(sizeof(It) == sizeof(void*));
    }

    friend std::uintptr_t id_key(iterator_id id) {
        return reinterpret_cast<std::uintptr_t>(std::addressof(*id.base()));
    }

    bool operator < (iterator_id rhs) const { return id_key(*this) < id_key(rhs); }

private:
    friend struct iterator_core_access;

    iterator_id dereference() const { return *this; }
};

template <class It>
inline size_t hash_value(const iterator_id<It>& id) {
    return hash_value(id_key(id));
}

template <class C>
struct container_id_traits<C, true, false> {
    using id       = iterator_id<typename C::const_iterator>;
    using iterator = id;
};

template <class C>
struct container_id_traits<C, true, true> {
    using id       = int;
    using iterator = integer_iterator<id>;
};

inline constexpr int id_key(int id) { return id; }

inline constexpr int id_to_raw(int id) { return id; }

template <class It>
inline void* id_to_raw(iterator_id<It> id) {
    return bit_cast<void*>(id);
}

/**
 * Container Id is int for random access containers and wrapped const_iterator otherwise.
 * Such Id isn't invalidated by emplace operation and can be used as map or hash map key.
 */
template <class C>
using container_id = typename container_id_traits<C>::id;

template <class C>
using container_id_iterator = typename container_id_traits<C>::iterator;

// This key can be used for id comparison.
template <class Id>
using id_key_t = decltype(id_key(std::declval<Id>()));

template <class C>
inline container_id<C> id_begin(const C& cont) {
    if constexpr (is_random_access_container_v<C>) {
        return 0;
    } else {
        return container_id<C>(cont.begin());
    }
}

template <class C>
inline container_id<C> id_end(const C& cont) {
    if constexpr (is_random_access_container_v<C>) {
        return static_cast<container_id<C>>(cont.size());
    } else {
        return container_id<C>(cont.end());
    }
}

// Returns an invalid Id that is fixed for the given container.
template <class C>
inline container_id<C> id_null(const C& cont) {
    if constexpr (is_random_access_container_v<C>) {
        return -1;
    } else {
        return id_end(cont);
    }
}

template <class C>
inline auto id_range(const C& cont) {
    using iterator = container_id_iterator<C>;
    return make_range(iterator(id_begin(cont)), iterator(id_end(cont)));
}

template <class C>
inline container_id<C> iterator_to_id(const C& cont, typename C::const_iterator it) {
    if constexpr (is_random_access_container_v<C>) {
        return static_cast<container_id<C>>(it - cont.begin());
    } else {
        return container_id<C>(it);
    }
}

template <class C>
inline typename C::const_iterator id_to_iterator(const C& cont, container_id<C> id) {
    if constexpr (is_random_access_container_v<C>) {
        ACTL_ASSERT(0 <= id && id < id_end(cont));
        return cont.begin() + id;
    } else {
        return id.base();
    }
}

template <class C>
inline typename C::reference id_at(C& cont, container_id<C> id) {
    if constexpr (is_random_access_container_v<C>) {
        return cont[static_cast<typename C::size_type>(id)];
    } else {
        // const_cast is required because id contains a const_iterator.
        // TODO: this cast allows modification of set key, which may lead to bugs.
        return const_cast<typename C::reference>(*id.base());
    }
}

template <class C>
inline typename C::const_reference id_at(const C& cont, container_id<C> id) {
    if constexpr (is_random_access_container_v<C>) {
        return cont[static_cast<typename C::size_type>(id)];
    } else {
        return *id.base();
    }
}

/* Generic container functions with id */

template <class C, class... Ts>
inline std::pair<container_id<C>, bool> id_emplace(C& cont, Ts&&... args) {
    auto res = emplace(cont, std::forward<Ts>(args)...);
    return {iterator_to_id(cont, res.first), res.second};
}

template <class C>
inline void id_erase(C& cont, container_id<C> id) {
    cont.erase(id_to_iterator(cont, id));
}

template <class C, class T>
inline container_id<C> id_find(const C& cont, const T& value) {
    return iterator_to_id(cont, find(cont, value));
}

}  // namespace ac

SPECIALIZE_STD_VALUE_HASH(ac::iterator_id)
