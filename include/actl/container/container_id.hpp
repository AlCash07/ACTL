// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/assert.hpp>
#include <actl/container/functions.hpp>
#include <actl/iterator/facade/iterator_adaptor.hpp>
#include <actl/iterator/integer_iterator.hpp>
#include <actl/numeric/bit.hpp>
#include <actl/numeric/util/hash_access.hpp>
#include <actl/range/iterator_range.hpp>
#include <actl/traits/dependent.hpp>
#include <cstdint>

namespace ac {

template <class C, bool = is_container_v<C>, bool = is_random_access_range_v<C>>
struct container_id_traits;

template <class It>
class iterator_id
    : public iterator_adaptor<iterator_id<It>, It, use_default, iterator_id<It>, iterator_id<It>> {
    using id = iterator_id;

public:
    explicit iterator_id(It it = It{})
        : iterator_adaptor<iterator_id<It>, It, use_default, id, id>{it} {}

    explicit iterator_id(void* raw)
        : iterator_adaptor<iterator_id<It>, It, use_default, id, id>{bit_cast<It>(raw)} {
        // TODO: implement more general logic in case this condition fails.
        static_assert(sizeof(It) == sizeof(void*));
    }

    friend std::uintptr_t get_id_key(iterator_id id) {
        return reinterpret_cast<std::uintptr_t>(std::addressof(*id.base()));
    }

    bool operator<(iterator_id rhs) const {
        return get_id_key(*this) < get_id_key(rhs);
    }

private:
    friend struct ac::iterator_core_access;
    friend struct ac::hash_access;

    iterator_id dereference() const {
        return *this;
    }

    size_t hash() const {
        return hash_value(get_id_key(*this));
    }
};

template <class C>
struct container_id_traits<C, true, false> {
    using id = iterator_id<iterator_t<const C>>;
    using iterator = id;
};

template <class C>
struct container_id_traits<C, true, true> {
    using id = int;
    using iterator = integer_iterator<id>;
};

inline constexpr int get_id_key(int id) {
    return id;
}

inline constexpr int id_to_raw(int id) {
    return id;
}

template <class It>
void* id_to_raw(iterator_id<It> id) {
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
using id_key_t = decltype(get_id_key(std::declval<Id>()));

template <class C>
container_id<C> id_begin(const C& cont) {
    if constexpr (is_random_access_range_v<C>) {
        return 0;
    } else {
        return container_id<C>{cont.begin()};
    }
}

template <class C>
container_id<C> id_end(const C& cont) {
    if constexpr (is_random_access_range_v<C>) {
        return static_cast<container_id<C>>(std::size(cont));
    } else {
        return container_id<C>{cont.end()};
    }
}

// Returns an invalid Id that is fixed for the given container.
template <class C>
container_id<C> id_null(const C& cont) {
    if constexpr (is_random_access_range_v<C>) {
        return -1;
    } else {
        return id_end(cont);
    }
}

template <class C>
auto id_range(const C& cont) {
    using iterator = container_id_iterator<C>;
    return make_range(iterator{id_begin(cont)}, iterator{id_end(cont)});
}

template <class C>
container_id<C> iterator_to_id(const C& cont, iterator_t<const C> it) {
    if constexpr (is_random_access_range_v<C>) {
        return static_cast<container_id<C>>(it - std::begin(cont));
    } else {
        return container_id<C>{it};
    }
}

template <class C>
iterator_t<const C> id_to_iterator(const C& cont, container_id<C> id) {
    if constexpr (is_random_access_range_v<C>) {
        ACTL_ASSERT(0 <= id && id <= id_end(cont));
        return std::begin(cont) + id;
    } else {
        return id.base();
    }
}

template <class C>
void id_check(C& cont, container_id<C> id) {
    if constexpr (is_random_access_range_v<C>) {
        ACTL_ASSERT(0 <= id && id < id_end(cont));
    } else {
        ACTL_ASSERT(id != id_end(cont));
    }
}

template <class C>
reference_t<C> id_at(C& cont, container_id<C> id) {
    id_check(cont, id);
    if constexpr (is_random_access_range_v<C>) {
        return cont[static_cast<size_type_t<C>>(id)];
    } else {
        // const_cast is required because id contains a const_iterator.
        // TODO: this cast allows modification of set key, which may lead to bugs.
        return const_cast<reference_t<C>>(*id.base());
    }
}

template <class C>
reference_t<const C> id_at(const C& cont, container_id<C> id) {
    id_check(cont, id);
    if constexpr (is_random_access_range_v<C>) {
        return cont[static_cast<size_type_t<C>>(id)];
    } else {
        return *id.base();
    }
}

/* Generic container functions with id */

template <class C, class... Ts>
std::pair<container_id<C>, bool> id_emplace(C& cont, Ts&&... args) {
    auto res = emplace(cont, std::forward<Ts>(args)...);
    return {iterator_to_id(cont, res.first), res.second};
}

template <class C>
void id_erase(C& cont, container_id<C> id) {
    cont.erase(id_to_iterator(cont, id));
}

template <class C, class T>
container_id<C> id_find(const C& cont, const T& value) {
    return iterator_to_id(cont, find(cont, value));
}

}  // namespace ac
