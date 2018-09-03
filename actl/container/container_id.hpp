/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/assert.hpp>
#include <actl/container/functions.hpp>
#include <actl/iterator/iterator_adaptor.hpp>
#include <actl/range/irange.hpp>
#include <actl/traits/container_traits.hpp>
#include <actl/traits/type_traits.hpp>
#include <algorithm>
#include <cstdint>

namespace ac {

template <class C, bool = is_container<C>::value, bool = is_random_access_v<C>>
struct container_id_traits;

template <class It>
class iterator_id : public It {};

template <class C>
struct container_id_traits<C, true, false> {
    using It = typename C::const_iterator;

    using id = iterator_id<It>;

    class iterator : public iterator_adaptor<iterator, id, use_default, id, id, id*> {
    public:
        explicit iterator(id value = {})
            : iterator_adaptor<iterator, id, use_default, id, id, id*>(value) {}

    private:
        friend struct ac::iterator_core_access;

        id dereference() const { return this->base(); }
    };

    friend id iterator_to_id(const C&, It it) { return it; }
    friend It id_to_iterator(const C&, id id) { return id; }

    friend constexpr std::uintptr_t get_id_key(id id) {
        return reinterpret_cast<std::uintptr_t>(std::addressof(*id));
    }

    friend bool operator < (id lhs, id rhs) { return get_id_key(lhs) < get_id_key(rhs); }

    friend id begin_id(const C& container) { return id(container.begin()); }
    friend id end_id(const C& container) { return id(container.end()); }
    // Returns an invalid Id that is fixed for the given container.
    friend id null_id(const C& container) { return end_id(container); }
};

template <class C>
struct container_id_traits<C, true, true> {
    using id       = int;
    using iterator = integer_iterator<id>;

    friend id iterator_to_id(const C& container, typename C::const_iterator it) {
        return static_cast<int>(it - container.begin());
    }

    friend typename C::const_iterator id_to_iterator(const C& container, id id) {
        ACTL_ASSERT(0 <= id && id < container.size());
        return container.begin() + id;
    }

    friend constexpr int get_id_key(int id) { return id; }

    friend id begin_id(const C&) { return 0; }
    friend id end_id(const C& container) { return static_cast<int>(container.size()); }
    friend id null_id(const C&) { return -1; }
};

constexpr int get_id_key(int id) { return id; }

/**
 * Container Id is int for random access containers and const_iterator otherwise.
 * Such Id isn't invalidated by emplace operation and can be used as map or hash map key.
 */
template <class C>
using container_id = typename container_id_traits<remove_cvref_t<C>>::id;

template <class C>
using container_id_iterator = typename container_id_traits<remove_cvref_t<C>>::iterator;

// This key can be used for id comparison.
template <class Id>
using id_key_t = decltype(get_id_key(std::declval<Id>()));

template <class C>
inline auto id_range(const C& container) {
    using iterator = container_id_iterator<C>;
    return make_range(iterator(begin_id(container)), iterator(end_id(container)));
}

/* Functions with id */

template <class C, class... Ts>
inline std::pair<container_id<C>, bool> id_emplace(C& container, Ts&&... args) {
    auto res = emplace(container, std::forward<Ts>(args)...);
    return {iterator_to_id(container, res.first), res.second};
}

template <class C>
inline void id_erase(C& container, container_id<C> id) {
    container.erase(id_to_iterator(container, id));
}

template <class C, class T>
inline container_id<C> id_find(C& container, const T& value) {
    return iterator_to_id(container, find(container, value));
}

}  // namespace ac

namespace std {

template <class It>
struct hash<ac::iterator_id<It>> {
    auto operator()(ac::iterator_id<It> id) const {
        return std::hash<std::uintptr_t>{}(ac::get_id_key(id));
    }
};

}  // namespace std
